#include "metric_impl/code_lines_count.hpp"
#include "file.hpp"
#include "function.hpp"

#include <gtest/gtest.h>

namespace {
const std::unordered_map<std::string, int> expected_metrics = {
    {"comments.py", 3},        {"exceptions.py", 7}, {"if.py", 3},        {"loops.py", 6},  {"many_lines.py", 11},
    {"many_parameters.py", 1}, {"match_case.py", 7}, {"nested_if.py", 8}, {"simple.py", 5}, {"ternary.py", 1},
};

}

namespace analyzer::metric::metric_impl {

TEST(CodeLinesCount, AllGood) {
    std::for_each(expected_metrics.cbegin(), expected_metrics.cend(), [](const auto &em) {
        auto functions = function::FunctionExtractor().Get(file::File(em.first));
        EXPECT_EQ(std::get<int>(CodeLinesCountMetric().Calculate(functions.front()).value), em.second);
    });
}

TEST(CodeLinesCount, IgnoresCommentOnlyLines) {
    const file::File source("comments.py");
    function::FunctionExtractor extractor;
    const auto functions = extractor.Get(source);

    ASSERT_EQ(1u, functions.size());

    const CodeLinesCountMetric metric;
    const auto result = metric.Calculate(functions.front());

    EXPECT_EQ(3, std::get<int>(result.value));
}

}  // namespace analyzer::metric::metric_impl