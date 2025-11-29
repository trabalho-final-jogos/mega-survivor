//
// Created by Lucas N. Ferreira on 08/12/23.
//

#pragma once

#include <string>

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

 private:
  // HUD elements
  std::string mFontName;
  float mXpProgress;
  UIText* mScore;
  UIText* mRunTime;
  UIRect* mXpBar;
};
