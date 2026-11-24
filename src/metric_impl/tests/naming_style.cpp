#include "metric_impl/naming_style.hpp"

#include <algorithm>
#include <unordered_map>

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {
namespace {

const std::unordered_map<std::string, std::string> expected_metrics = {
    {"comments.py", "Unknown"},   {"exceptions.py", "Unknown"},    {"if.py", "Camel Case"},
    {"loops.py", "Pascal Case"},  {"many_lines.py", "Lower Case"}, {"many_parameters.py", "Snake Case"},
    {"match_case.py", "Unknown"}, {"nested_if.py", "Pascal Case"}, {"simple.py", "Snake Case"},
    {"ternary.py", "Unknown"},
};

}  // namespace

TEST(NamingStyleMetric, AllGood) {
    std::for_each(expected_metrics.cbegin(), expected_metrics.cend(), [](const auto &entry) {
        const auto &[filename, expected] = entry;
        SCOPED_TRACE(filename);
        const auto functions = function::FunctionExtractor().Get(file::File(filename));
        ASSERT_EQ(1u, functions.size());
        EXPECT_EQ(expected, std::get<std::string>(NamingStyleMetric().Calculate(functions.front()).value));
    });
}

}  // namespace analyzer::metric::metric_impl
