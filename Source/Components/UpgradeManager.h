#pragma once
#include <cstdint>
#include <unordered_map>

constexpr float ADDITIONAL_MULTIPLIER_PER_UPGRADE{0.2f};
constexpr int ADDITIONAL_AMOUNT_PER_UPGRADE{5};
constexpr int BASE_CURRENCY_INCREASE_PER_LEVEL{500};

enum class StatType { Speed, Damage, Area, Projectiles, Regen, Lucky, Health };

struct PlayerStatsUpgrade {
  uint8_t speedMultiplier;
  uint8_t damageMultiplier;
  uint8_t areaMultiplier;
  uint8_t projectilesAmountIncrease;
  uint8_t regenRateIncrease;
  uint8_t luckyChanceIncrease;
  uint8_t healthIncrease;
};

class UpgradeManager {
 public:
  static UpgradeManager& GetInstance();

  bool CanAfford(StatType type) const;
  void Purchase(StatType type);

  const PlayerStatsUpgrade& GetUpgrades() const { return mUpgrades; }
  int GetCurrency() const { return mCurrency; }
  void AddCurrency(int amount) { mCurrency += amount; }

  uint8_t GetUpgradeLevel(StatType type) const;
  int GetUpgradeCost(StatType type) const;

  void UpgradeStat(StatType type);
  // Serialize to JSON/file for persistence (nlohmann/json)

 private:
  UpgradeManager();
  UpgradeManager(const UpgradeManager&) = delete;
  UpgradeManager& operator=(const UpgradeManager&) = delete;

  PlayerStatsUpgrade mUpgrades{};
  int mCurrency{1000};
};
