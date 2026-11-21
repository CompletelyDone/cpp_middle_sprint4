#include <unistd.h>

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "file.hpp"
#include "function.hpp"
#include "metric.hpp"
#include "metric_accumulator.hpp"

namespace analyzer {

namespace rv = std::ranges::views;
namespace rs = std::ranges;
/**
 * @brief Анализирует список Python-файлов и извлекает метрики для всех функций и методов.
 *
 * Эта функция — центральный "конвейер" обработки:
 * 1. Принимает имена файлов.
 * 2. Для каждого файла создаёт объект `File`, который автоматически парсит его через tree-sitter
 *    и строит AST.
 * 3. Извлекает из AST все функции и методы с помощью `FunctionExtractor`.
 * 4. Объединяет все функции из всех файлов в один плоский список (`join`).
 * 5. Для каждой функции вычисляет набор метрик через переданный `metric_extractor`.
 * 6. Возвращает вектор пар: (функция, результаты её метрик).
 */
auto AnalyseFunctions(const std::vector<std::string> &files,
                      const analyzer::metric::MetricExtractor &metric_extractor) {
    // здесь ваш код
    const auto parsed_files =
        std::views::transform(files, [](const std::string &filename) { return file::File{filename}; });

    const auto functions_by_file = std::views::transform(parsed_files, [](const auto &file) {
        function::FunctionExtractor extractor;
        return extractor.Get(file);
    });

    auto joined = std::views::join(functions_by_file);

    auto metrics_by_func = std::views::transform(joined, [&](const function::Function &f) {
        return std::pair<function::Function, metric::MetricResults>(f, metric_extractor.Get(f));
    });

    return std::ranges::to<std::vector<std::pair<function::Function, metric::MetricResults>>>(metrics_by_func);
}

/**
 *
 * @brief Группирует результаты анализа по классам.
 *
 * Эта функция:
 * 1. Отфильтровывает только те функции, которые являются **методами классов**
 *    (у них `class_name.has_value()` == true).
 * 2. Группирует последовательные элементы с одинаковым именем класса с помощью `chunk_by`.
 *
 * Важно:
 * - `chunk_by` работает только с **последовательными** одинаковыми элементами!
 *   Поэтому предполагается, что входной диапазон уже упорядочен по классам
 *   (например, порядок методов в AST сохраняется как в исходном файле).
 * - Если порядок нарушен, один и тот же класс может быть разбит на несколько групп.
 *
 *  Чтобы убедиться, что фильтрация работает, проверьте, что свободные функции (без class_name)
 * действительно исчезают из результата.
 */
auto SplitByClasses(const auto &analysis) {
    // здесь ваш код

    auto class_methods = std::views::filter(analysis, [](const auto &el) { return el.first.class_name.has_value(); });

    return std::views::chunk_by(class_methods, [](const auto &a, const auto &b) {
        return std::tie(a.first.class_name, a.first.filename) == std::tie(b.first.class_name, b.first.filename);
    });
}

/**
 * @brief Группирует результаты анализа по исходным файлам.
 *
 * Эта функция:
 * - Разбивает весь список функций на группы, где каждая группа содержит
 *   только функции из одного и того же файла (`filename`).
 * - Использует `chunk_by`, поэтому **порядок функций в `analysis` должен быть по файлам**.
 */
auto SplitByFiles(const auto &analysis) {
    // здесь ваш код
    return std::views::chunk_by(analysis,
                                [](const auto &a, const auto &b) { return a.first.filename == b.first.filename; });
}

/**
 * @brief Агрегирует метрики всех функций с помощью аккумулятора.
 *
 * Эта функция:
 * - Проходит по каждому элементу результата `AnalyseFunctions`
 *   (то есть по каждой функции и её метрикам).
 * - Передаёт результаты метрик (`elem.second`) в аккумулятор через `AccumulateNextFunctionResults`.
 */
void AccumulateFunctionAnalysis(const auto &analysis,
                                const analyzer::metric_accumulator::MetricsAccumulator &accumulator) {
    // здесь ваш код
    const auto values = std::views::values(analysis);

    std::ranges::for_each(values, [&](const auto &metrics) { accumulator.AccumulateNextFunctionResults(metrics); });
}

}  // namespace analyzer
