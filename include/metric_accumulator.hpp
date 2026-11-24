#pragma once
#include <stdexcept>
#include <unistd.h>

#include <algorithm>
#include <any>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

#include "metric.hpp"

namespace rv = std::ranges::views;
namespace rs = std::ranges;

namespace analyzer::metric_accumulator {

struct IAccumulator {
    virtual void Accumulate(const metric::MetricResult &metric_result) = 0;
    virtual void Finalize() = 0;
    virtual void Reset() = 0;
    virtual ~IAccumulator() = default;

protected:
    bool is_finalized = false;
};

struct MetricsAccumulator {
    template <typename Accumulator>
    void RegisterAccumulator(const std::string &metric_name, std::unique_ptr<Accumulator> acc) {
        if (metric_name.empty()) {
            throw std::runtime_error("MetricsAccumulator::RegisterAccumulator: metric name empty");
        }

        if (!acc) {
            throw std::runtime_error("MetricsAccumulator::RegisterAccumulator: acc is null");
        }

        const auto pair = accumulators.emplace(metric_name, std::move(acc));
        if (pair.second == false) {
            throw std::runtime_error("MetricsAccumulator::RegisterAccumulator: acc is already exist");
        }
    }
    template <typename Accumulator>
    const Accumulator &GetFinalizedAccumulator(const std::string &metric_name) const {
        if (metric_name.empty())
            throw std::runtime_error("MetricsAccumulator::GetFinalizedAccumulator: metric name empty");

        auto &metric_accululator = *accumulators.at(metric_name);
        const auto &result = dynamic_cast<const Accumulator &>(metric_accululator);

        metric_accululator.Finalize();

        return result;
    }
    void AccumulateNextFunctionResults(const std::vector<metric::MetricResult> &metric_results) const;

    void ResetAccumulators();

private:
    std::unordered_map<std::string, std::shared_ptr<IAccumulator>> accumulators;
};

}  // namespace analyzer::metric_accumulator
