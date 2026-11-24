#include "metric_impl/code_lines_count.hpp"

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

    const auto result = CodeLinesCountMetric().Calculate(functions.front());
    EXPECT_EQ(expected, std::get<int>(result.value));
}

}  // namespace

TEST(CodeLinesCount, IgnoresCommentOnlyLines) { CheckMetric("comments.py", 3); }
TEST(CodeLinesCount, Exceptions) { CheckMetric("exceptions.py", 7); }
TEST(CodeLinesCount, If) { CheckMetric("if.py", 3); }
TEST(CodeLinesCount, Loops) { CheckMetric("loops.py", 6); }
TEST(CodeLinesCount, ManyLines) { CheckMetric("many_lines.py", 11); }
TEST(CodeLinesCount, ManyParameters) { CheckMetric("many_parameters.py", 1); }
TEST(CodeLinesCount, MatchCase) { CheckMetric("match_case.py", 7); }
TEST(CodeLinesCount, NestedIf) { CheckMetric("nested_if.py", 8); }
TEST(CodeLinesCount, Simple) { CheckMetric("simple.py", 5); }
TEST(CodeLinesCount, Ternary) { CheckMetric("ternary.py", 1); }

}  // namespace analyzer::metric::metric_impl
