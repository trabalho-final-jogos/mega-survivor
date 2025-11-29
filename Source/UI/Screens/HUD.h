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

 private:
  // HUD elements
  UIImage* mXpBar;
  UIText* mScore;
  UIText* mRunTime;
};
