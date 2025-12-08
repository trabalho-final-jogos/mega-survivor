#include "UpgradeComponent.h"
#include "../../Game.h"
#include "../Component.h"

UpgradeComponent::UpgradeComponent(Actor* owner) : Component(owner) {
  base_stats_.fill(0.0f);
  run_stats_.fill(0.0f);
  final_stats_.fill(0.0f);
  base_levels_.fill(0);
}

UpgradeComponent::~UpgradeComponent() {}

nlohmann::json UpgradeComponent::Save() const {
  nlohmann::json j;
  j["base_levels"] = base_levels_;
  j["base_stats"] = base_stats_;
  return j;
}

void UpgradeComponent::Load(const nlohmann::json& j) {
  if (j.contains("base_levels")) {
    base_levels_ =
        j["base_levels"]
            .get<std::array<int, static_cast<size_t>(Stats::Count)>>();

    SDL_Log("Base levels loaded");
  }
  if (j.contains("base_stats")) {
    base_stats_ =
        j["base_stats"]
            .get<std::array<float, static_cast<size_t>(Stats::Count)>>();
    SDL_Log("Base stats loaded");
  }
  // Recalculate final stats
  for (int i = 0; i < static_cast<int>(Stats::Count); ++i) {
    RecalculateStat(static_cast<Stats>(i));
  }
}