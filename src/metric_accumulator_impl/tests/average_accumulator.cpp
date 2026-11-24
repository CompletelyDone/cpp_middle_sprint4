#include "metric_accumulator_impl/average_accumulator.hpp"
#include "metric.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {

namespace {

metric::MetricResult MakeResult(int value) { return {"metric", value}; }

}  // namespace

TEST(AverageAccumulator, ThrowBeforeFinalize) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(1));
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(AverageAccumulator, ThrowOnEmptyAcc) {
    AverageAccumulator acc;
    EXPECT_THROW(acc.Finalize(), std::runtime_error);
}

TEST(AverageAccumulator, ReturnsSingleValue) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(5));
    acc.Finalize();
    EXPECT_DOUBLE_EQ(5.0, acc.Get());
}

TEST(AverageAccumulator, AveragesSeveralValues) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(3));
    acc.Accumulate(MakeResult(9));
    acc.Accumulate(MakeResult(6));
    acc.Finalize();
    EXPECT_DOUBLE_EQ(6.0, acc.Get());
}

TEST(AverageAccumulator, ReturnsFractionalAverage) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(1));
    acc.Accumulate(MakeResult(2));
    acc.Finalize();
    EXPECT_DOUBLE_EQ(1.5, acc.Get());
}

TEST(AverageAccumulator, AveragesNegativeValues) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(-2));
    acc.Accumulate(MakeResult(-6));
    acc.Finalize();
    EXPECT_DOUBLE_EQ(-4.0, acc.Get());
}

TEST(AverageAccumulator, ResetClearsPreviousValues) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(10));
    acc.Finalize();

    acc.Reset();
    acc.Accumulate(MakeResult(2));
    acc.Finalize();
    EXPECT_DOUBLE_EQ(2.0, acc.Get());
}

TEST(AverageAccumulator, ThrowOnAccumulateAfterFinalize) {
    AverageAccumulator acc;
    acc.Accumulate(MakeResult(1));
    acc.Finalize();
    EXPECT_THROW(acc.Accumulate(MakeResult(2)), std::runtime_error);
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
