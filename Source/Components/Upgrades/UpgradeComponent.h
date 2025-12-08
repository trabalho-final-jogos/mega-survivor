#pragma once

#include <algorithm>
#include <array>
#include <vector>
#include "../../Json.h"
#include "../Component.h"

constexpr float ADDITIONAL_MULTIPLIER_PER_UPGRADE{0.05f};
constexpr int ADDITIONAL_AMOUNT_PER_UPGRADE{5};
constexpr int BASE_CURRENCY_INCREASE_PER_LEVEL{500};

enum class Stats {
  Speed,
  Damage,
  Area,
  Projectiles,
  Regen,
  Lucky,
  Health,
  Count,
  None  // Must be the last one
};

class UpgradeComponent : public Component {
 public:
  UpgradeComponent(class Actor* owner);
  ~UpgradeComponent();

  int GetBaseLevel(Stats stat) const {
    return base_levels_[static_cast<size_t>(stat)];
  }

  int GetUpgradeCost(Stats stat) const {
    return (GetBaseLevel(stat) + 1) * BASE_CURRENCY_INCREASE_PER_LEVEL;
  }

  void UpgradeBaseStat(Stats stat, float amount) {
    size_t idx = static_cast<size_t>(stat);
    base_stats_[idx] += amount;
    base_levels_[idx]++;
    RecalculateStat(stat);
  };

  void UpgradeRunStat(Stats stat, float amount) {
    run_stats_[static_cast<size_t>(stat)] += amount;
    RecalculateStat(stat);
  };

  float GetFinalStat(Stats stat) const {
    return final_stats_[static_cast<size_t>(stat)];
  }

  void CopyBaseStatsFrom(const UpgradeComponent& other) {
    base_stats_ = other.base_stats_;
    base_levels_ = other.base_levels_;
    // Recalculate all
    for (int i = 0; i < static_cast<int>(Stats::Count); ++i) {
      RecalculateStat(static_cast<Stats>(i));
    }
  }

  nlohmann::json Save() const;
  void Load(const nlohmann::json& j);

 private:
  void RecalculateStat(Stats stat) {
    const size_t idx = static_cast<size_t>(stat);
    final_stats_[idx] = base_stats_[idx] + run_stats_[idx];
  }

  std::array<float, static_cast<size_t>(Stats::Count)> final_stats_;
  std::array<float, static_cast<size_t>(Stats::Count)> base_stats_;
  std::array<float, static_cast<size_t>(Stats::Count)> run_stats_;

  std::array<int, static_cast<size_t>(Stats::Count)> base_levels_;
};
