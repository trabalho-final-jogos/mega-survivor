#pragma once

#include <algorithm>
#include <array>
#include <vector>
#include "../Component.h"

enum class Stats {
  Speed,
  Damage,
  Area,
  Projectiles,
  Regen,
  Lucky,
  Health,
  Count  // Must be the last one
};

class UpgradeComponent {
 public:
  UpgradeComponent();
  ~UpgradeComponent();

  void UpgradeBaseStat(Stats stat, float amount) {
    base_stats_[static_cast<size_t>(stat)] += amount;
    RecalculateStat(stat);
  };

  void UpgradeRunStat(Stats stat, float amount) {
    run_stats_[static_cast<size_t>(stat)] += amount;
    RecalculateStat(stat);
  };

  float GetFinalStat(Stats stat) const {
    return final_stats_[static_cast<size_t>(stat)];
  }

 private:
  void RecalculateStat(Stats stat) {
    const size_t idx = static_cast<size_t>(stat);
    final_stats_[idx] = base_stats_[idx] + run_stats_[idx];
  }

  std::array<float, static_cast<size_t>(Stats::Count)> final_stats_;
  std::array<float, static_cast<size_t>(Stats::Count)> base_stats_;
  std::array<float, static_cast<size_t>(Stats::Count)> run_stats_;
};