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
