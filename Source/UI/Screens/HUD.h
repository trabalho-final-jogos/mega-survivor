//
// Created by Lucas N. Ferreira on 08/12/23.
//

#pragma once

#include <string>

#include "../../Actors/Player.h"
#include "../../Color.h"
#include "../../Managers/ColorPalette.h"
#include "UIScreen.h"

class HUD : public UIScreen {
 public:
  HUD(class Game* game, const std::string& fontName);

  void SetScore(int score);
  void HandleKeyPress(int key) override;
  void SetXPBar(float percentage);
  void Update(float deltaTime) override;
  Vector2 GetXPBarSize(float bar_progress);
  Vector2 GetXPBarOffset(float bar_progress);
  void ShowFlash();
  void UpdateHealthBar(const Player* player);

 private:
  // HUD elements
  std::string mFontName;
  float mDisplayedProgress{0.0f};
  float mFlashTimer{0.0f};
  Vector3 mFlashColor;

  UIText* mScore;
  UIText* mRunTime;
  UIRect* mXpBar;
  UIRect* mOverlay;
  uint32_t mLastPlayerHP{20};

  UIRect* mHealthBar;
  UIRect* mHealthBarBg;
};
