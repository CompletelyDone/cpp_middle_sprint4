#include "metric_accumulator_impl/categorical_accumulator.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>

#include <gtest/gtest.h>

namespace analyzer::metric_accumulator::metric_accumulator_impl::test {
namespace {

metric::MetricResult MakeResult(const std::string &value) { return {"metric", value}; }

}  // namespace

TEST(CategoricalAccumulator, ThrowBeforeFinalize) {
    CategoricalAccumulator acc;
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(CategoricalAccumulator, ReturnsNoCategoriesForEmptyAccumulator) {
    CategoricalAccumulator acc;
    acc.Finalize();
    EXPECT_TRUE(acc.Get().empty());
}

TEST(CategoricalAccumulator, CountsSingleCategory) {
    CategoricalAccumulator acc;
    acc.Accumulate(MakeResult("Snake Case"));
    acc.Finalize();
    EXPECT_EQ(1, acc.Get().at("Snake Case"));
}

TEST(CategoricalAccumulator, CountsRepeatedCategory) {
    CategoricalAccumulator acc;
    acc.Accumulate(MakeResult("Snake Case"));
    acc.Accumulate(MakeResult("Snake Case"));
    acc.Finalize();
    EXPECT_EQ(2, acc.Get().at("Snake Case"));
}

TEST(CategoricalAccumulator, CountsDistinctCategoriesSeparately) {
    CategoricalAccumulator acc;
    acc.Accumulate(MakeResult("Snake Case"));
    acc.Accumulate(MakeResult("Camel Case"));
    acc.Finalize();
    const std::unordered_map<std::string, int> expected{{"Snake Case", 1}, {"Camel Case", 1}};
    EXPECT_EQ(expected, acc.Get());
}

TEST(CategoricalAccumulator, ResetClearsCategories) {
    CategoricalAccumulator acc;
    acc.Accumulate(MakeResult("Snake Case"));
    acc.Finalize();
    acc.Reset();
    acc.Finalize();
    EXPECT_TRUE(acc.Get().empty());
}

TEST(CategoricalAccumulator, ThrowAfterReset) {
    CategoricalAccumulator acc;
    acc.Finalize();
    acc.Reset();
    EXPECT_THROW(acc.Get(), std::runtime_error);
}

TEST(CategoricalAccumulator, CountsCategoryFromZeroAfterReset) {
    CategoricalAccumulator acc;
    acc.Accumulate(MakeResult("Snake Case"));
    acc.Finalize();

    acc.Reset();
    acc.Accumulate(MakeResult("Snake Case"));
    acc.Finalize();
    EXPECT_EQ(1, acc.Get().at("Snake Case"));
}

}  // namespace analyzer::metric_accumulator::metric_accumulator_impl::test
