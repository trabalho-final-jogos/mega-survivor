#include "UpgradeComponent.h"
#include "../../Game.h"
#include "../Component.h"

UpgradeComponent::UpgradeComponent() {
  base_stats_.fill(0.0f);
  run_stats_.fill(0.0f);
  final_stats_.fill(0.0f);
}

UpgradeComponent::~UpgradeComponent() {}
