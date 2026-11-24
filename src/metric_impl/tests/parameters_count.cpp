#include "metric_impl/parameters_count.hpp"

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

    const auto result = CountParametersMetric().Calculate(functions.front());
    EXPECT_EQ(expected, std::get<int>(result.value));
}

}  // namespace

TEST(CountParametersMetric, Comments) { CheckMetric("comments.py", 3); }
TEST(CountParametersMetric, Exceptions) { CheckMetric("exceptions.py", 0); }
TEST(CountParametersMetric, If) { CheckMetric("if.py", 1); }
TEST(CountParametersMetric, Loops) { CheckMetric("loops.py", 1); }
TEST(CountParametersMetric, ManyLines) { CheckMetric("many_lines.py", 0); }
TEST(CountParametersMetric, ManyParameters) { CheckMetric("many_parameters.py", 5); }
TEST(CountParametersMetric, MatchCase) { CheckMetric("match_case.py", 1); }
TEST(CountParametersMetric, NestedIf) { CheckMetric("nested_if.py", 2); }
TEST(CountParametersMetric, Simple) { CheckMetric("simple.py", 0); }
TEST(CountParametersMetric, Ternary) { CheckMetric("ternary.py", 1); }

}  // namespace analyzer::metric::metric_impl
