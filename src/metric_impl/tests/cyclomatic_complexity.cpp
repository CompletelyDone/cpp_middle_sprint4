#include "metric_impl/cyclomatic_complexity.hpp"

#include <string>

#include <gtest/gtest.h>

#include "file.hpp"
#include "function.hpp"

namespace analyzer::metric::metric_impl {
namespace {

void CheckMetric(const std::string &filename, int expected) {
    const file::File source(filename);
    const auto functions = function::FunctionExtractor().Get(source);
    ASSERT_EQ(1u, functions.size());

    const auto result = CyclomaticComplexityMetric().Calculate(functions.front());
    EXPECT_EQ(expected, std::get<int>(result.value));
}

}  // namespace

TEST(CyclomaticComplexityMetric, Comments) { CheckMetric("comments.py", 1); }
TEST(CyclomaticComplexityMetric, Exceptions) { CheckMetric("exceptions.py", 4); }
TEST(CyclomaticComplexityMetric, If) { CheckMetric("if.py", 2); }
TEST(CyclomaticComplexityMetric, Loops) { CheckMetric("loops.py", 4); }
TEST(CyclomaticComplexityMetric, ManyLines) { CheckMetric("many_lines.py", 2); }
TEST(CyclomaticComplexityMetric, ManyParameters) { CheckMetric("many_parameters.py", 2); }
TEST(CyclomaticComplexityMetric, MatchCase) { CheckMetric("match_case.py", 4); }
TEST(CyclomaticComplexityMetric, NestedIf) { CheckMetric("nested_if.py", 4); }
TEST(CyclomaticComplexityMetric, Simple) { CheckMetric("simple.py", 2); }
TEST(CyclomaticComplexityMetric, Ternary) { CheckMetric("ternary.py", 3); }

}  // namespace analyzer::metric::metric_impl
