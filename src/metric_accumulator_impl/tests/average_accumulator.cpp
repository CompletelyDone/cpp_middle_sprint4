#include "metric_accumulator_impl/average_accumulator.hpp"
#include "metric.hpp"

#include <gtest/gtest.h>

#include <cmath>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

// здесь ваш код
TEST(AverageAccumulator, ThrowBeforeFinalize) {
    AverageAccumulator acc;
    acc.Accumulate(metric::MetricResult{"metric", 1});
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(AverageAccumulator, ThrowOnEmptyAcc) {
    AverageAccumulator acc;
    EXPECT_THROW(acc.Finalize(), std::runtime_error);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
