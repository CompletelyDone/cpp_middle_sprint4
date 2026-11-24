#include "metric_impl/naming_style.hpp"

#include <string>

#include <gtest/gtest.h>

#include "file.hpp"
#include "function.hpp"

namespace analyzer::metric::metric_impl {
namespace {

void CheckMetric(const std::string &filename, const std::string &expected) {
    const file::File source(filename);
    const auto functions = function::FunctionExtractor().Get(source);
    ASSERT_EQ(1u, functions.size());

    const auto result = NamingStyleMetric().Calculate(functions.front());
    EXPECT_EQ(expected, std::get<std::string>(result.value));
}

}  // namespace

TEST(NamingStyleMetric, Comments) { CheckMetric("comments.py", "Unknown"); }
TEST(NamingStyleMetric, Exceptions) { CheckMetric("exceptions.py", "Unknown"); }
TEST(NamingStyleMetric, If) { CheckMetric("if.py", "Camel Case"); }
TEST(NamingStyleMetric, Loops) { CheckMetric("loops.py", "Pascal Case"); }
TEST(NamingStyleMetric, ManyLines) { CheckMetric("many_lines.py", "Lower Case"); }
TEST(NamingStyleMetric, ManyParameters) { CheckMetric("many_parameters.py", "Snake Case"); }
TEST(NamingStyleMetric, MatchCase) { CheckMetric("match_case.py", "Unknown"); }
TEST(NamingStyleMetric, NestedIf) { CheckMetric("nested_if.py", "Pascal Case"); }
TEST(NamingStyleMetric, Simple) { CheckMetric("simple.py", "Snake Case"); }
TEST(NamingStyleMetric, Ternary) { CheckMetric("ternary.py", "Unknown"); }

}  // namespace analyzer::metric::metric_impl
