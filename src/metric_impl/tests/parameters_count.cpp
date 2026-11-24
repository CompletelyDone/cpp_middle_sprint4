#include "metric_impl/parameters_count.hpp"

#include <algorithm>
#include <unordered_map>

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {
namespace {

const std::unordered_map<std::string, int> expected_metrics = {
    {"comments.py", 3},        {"exceptions.py", 0}, {"if.py", 1},        {"loops.py", 1},  {"many_lines.py", 0},
    {"many_parameters.py", 5}, {"match_case.py", 1}, {"nested_if.py", 2}, {"simple.py", 0}, {"ternary.py", 1},
};

}  // namespace

TEST(CountParametersMetric, AllGood) {
    std::for_each(expected_metrics.cbegin(), expected_metrics.cend(), [](const auto &entry) {
        const auto &[filename, expected] = entry;
        SCOPED_TRACE(filename);
        const auto functions = function::FunctionExtractor().Get(file::File(filename));
        ASSERT_EQ(1u, functions.size());
        EXPECT_EQ(expected, std::get<int>(CountParametersMetric().Calculate(functions.front()).value));
    });
}

}  // namespace analyzer::metric::metric_impl
