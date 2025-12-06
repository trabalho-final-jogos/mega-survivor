#pragma once
#include <array>
#include "../../Components/Upgrades/UpgradeComponent.h"
#include "UIScreen.h"

const int cols = 3;
const int rows = 4;
const float startX = -150.0f;
const float startY = 50.0f;
const float spacingX = 175.0f;
const float spacingY = 100.0f;

class UpgradeStore : public UIScreen {
 public:
  UpgradeStore(class Game* game, const std::string& fontName);
  void HandleKeyPress(int key);
  void Update(float deltaTime) override;

 private:
  UIText* mCurrencyText = nullptr;
  std::array<class UIButton*, 7> mUpgradeButtons{nullptr};

  std::array<std::pair<std::string, Stats>, 7> statButtons = {
      {{"Speed", Stats::Speed},
       {"Damage", Stats::Damage},
       {"Area", Stats::Area},
       {"Projec", Stats::Projectiles},
       {"Regen", Stats::Regen},
       {"Lucky", Stats::Lucky},
       {"Health", Stats::Health}}};
};
