#include "metric_accumulator_impl/sum_average_accumulator.hpp"

#include <stdexcept>

#include <gtest/gtest.h>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {
namespace {

metric::MetricResult MakeResult(int value) { return {"metric", value}; }

}  // namespace

TEST(SumAverageAccumulator, ThrowBeforeFinalize) {
    SumAverageAccumulator acc;
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(SumAverageAccumulator, SumsSingleValue) {
    SumAverageAccumulator acc;
    acc.Accumulate(MakeResult(7));
    acc.Finalize();
    EXPECT_EQ(7, acc.Get().sum);
}

TEST(SumAverageAccumulator, SumsSeveralValues) {
    SumAverageAccumulator acc;
    acc.Accumulate(MakeResult(3));
    acc.Accumulate(MakeResult(5));
    acc.Accumulate(MakeResult(7));
    acc.Finalize();
    EXPECT_EQ(15, acc.Get().sum);
}

TEST(SumAverageAccumulator, SumsNegativeAndPositiveValues) {
    SumAverageAccumulator acc;
    acc.Accumulate(MakeResult(-5));
    acc.Accumulate(MakeResult(3));
    acc.Finalize();
    EXPECT_EQ(-2, acc.Get().sum);
}

TEST(SumAverageAccumulator, AveragesSeveralValues) {
    SumAverageAccumulator acc;
    acc.Accumulate(MakeResult(4));
    acc.Accumulate(MakeResult(8));
    acc.Finalize();
    EXPECT_DOUBLE_EQ(6.0, acc.Get().average);
}

TEST(SumAverageAccumulator, ReturnsFractionalAverage) {
    SumAverageAccumulator acc;
    acc.Accumulate(MakeResult(2));
    acc.Accumulate(MakeResult(3));
    acc.Finalize();
    EXPECT_DOUBLE_EQ(2.5, acc.Get().average);
}

TEST(SumAverageAccumulator, ResetClearsPreviousValues) {
    SumAverageAccumulator acc;
    acc.Accumulate(MakeResult(10));
    acc.Finalize();

    acc.Reset();
    acc.Accumulate(MakeResult(3));
    acc.Finalize();
    EXPECT_EQ(3, acc.Get().sum);
}

TEST(SumAverageAccumulator, ThrowAfterReset) {
    SumAverageAccumulator acc;
    acc.Accumulate(MakeResult(1));
    acc.Finalize();
    acc.Reset();
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
