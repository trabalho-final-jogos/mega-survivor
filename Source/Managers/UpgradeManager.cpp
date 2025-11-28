#include "UpgradeManager.h"

UpgradeManager::UpgradeManager() {}

UpgradeManager& UpgradeManager::GetInstance() {
  static UpgradeManager instance;
  return instance;
}

bool UpgradeManager::CanAfford(StatType type) const {
  return (GetUpgradeCost(type) <= mCurrency);
}

int UpgradeManager::GetUpgradeCost(StatType type) const {
  return ((GetUpgradeLevel(type) + 1) * BASE_CURRENCY_INCREASE_PER_LEVEL);
}

uint8_t UpgradeManager::GetUpgradeLevel(StatType type) const {
  switch (type) {
    case StatType::Speed:
      return mUpgrades.speedMultiplier;
    case StatType::Damage:
      return mUpgrades.damageMultiplier;
    case StatType::Area:
      return mUpgrades.areaMultiplier;
    case StatType::Projectiles:
      return mUpgrades.projectilesAmountIncrease;
    case StatType::Regen:
      return mUpgrades.regenRateIncrease;
    case StatType::Lucky:
      return mUpgrades.luckyChanceIncrease;
    case StatType::Health:
      return mUpgrades.healthIncrease;
    default:
      return 0;  // Invalid type
  }
}

void UpgradeManager::UpgradeStat(StatType type) {
  switch (type) {
    case StatType::Speed:
      mUpgrades.speedMultiplier += 1;
      break;
    case StatType::Damage:
      mUpgrades.damageMultiplier += 1;
      break;
    case StatType::Area:
      mUpgrades.areaMultiplier += 1;
      break;
    case StatType::Projectiles:
      mUpgrades.projectilesAmountIncrease += 1;
      break;
    case StatType::Regen:
      mUpgrades.regenRateIncrease += 1;
      break;
    case StatType::Lucky:
      mUpgrades.luckyChanceIncrease += 1;
      break;
    case StatType::Health:
      mUpgrades.healthIncrease += 1;
      break;
    default:
      break;
  }
}

void UpgradeManager::Purchase(StatType type) {
  int cost = GetUpgradeCost(type);
  if (mCurrency >= cost) {
    mCurrency -= cost;
    UpgradeStat(type);
  }
}