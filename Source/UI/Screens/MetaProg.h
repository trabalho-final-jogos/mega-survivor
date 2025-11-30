#pragma once
#include <array>
#include "../../Managers/UpgradeManager.h"
#include "UIScreen.h"

const int cols = 3;
const int rows = 4;
const float startX = -150.0f;
const float startY = 50.0f;
const float spacingX = 175.0f;
const float spacingY = 100.0f;

class MetaProg : public UIScreen {
 public:
  MetaProg(class Game* game, const std::string& fontName);
  void HandleKeyPress(int key);
  void Update(float deltaTime) override;

 private:
  UIText* mCurrencyText = nullptr;
  std::array<class UIButton*, 7> mUpgradeButtons{nullptr};

  std::array<std::pair<std::string, StatType>, 7> statButtons = {
      {{"Speed", StatType::Speed},
       {"Damage", StatType::Damage},
       {"Area", StatType::Area},
       {"Projec", StatType::Projectiles},
       {"Regen", StatType::Regen},
       {"Lucky", StatType::Lucky},
       {"Health", StatType::Health}}};
};
