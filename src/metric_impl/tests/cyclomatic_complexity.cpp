#include "metric_impl/cyclomatic_complexity.hpp"

#include <gtest/gtest.h>

namespace {
const std::unordered_map<std::string, int> expected_metrics = {
    {"comments.py", 1},        {"exceptions.py", 4}, {"if.py", 2},        {"loops.py", 4},  {"many_lines.py", 2},
    {"many_parameters.py", 2}, {"match_case.py", 4}, {"nested_if.py", 4}, {"simple.py", 2}, {"ternary.py", 3}};

}

namespace analyzer::metric::metric_impl {

// здесь ваш код

TEST(CyclomaticComplexityMetric, AllGood) {
    std::for_each(expected_metrics.cbegin(), expected_metrics.cend(), [](const auto &em) {
        auto functions = function::FunctionExtractor().Get(file::File(em.first));
        EXPECT_EQ(std::get<int>(CyclomaticComplexityMetric().Calculate(functions.front()).value), em.second);
    });
}

}  // namespace analyzer::metric::metric_impl
