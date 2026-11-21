#include "metric_impl/code_lines_count.hpp"
#include "file.hpp"
#include "function.hpp"

#include <gtest/gtest.h>

namespace analyzer::metric::metric_impl {

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