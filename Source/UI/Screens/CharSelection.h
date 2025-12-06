//
// Created by daniel on 29/11/2025.
//

#ifndef MEGA_SURVIVOR_CHARSELECTION_H
#define MEGA_SURVIVOR_CHARSELECTION_H
#include "../../Actors/Player.h"
#include "UIScreen.h"

class CharSelection : public UIScreen {
 public:
  CharSelection(class Game* game, const std::string& fontName);
  void HandleKeyPress(int key) override;
  // void Update(float deltaTime) override;
 private:
  std::array<class UIButton*, kCharCount> mCharButtons{nullptr};
  PlayerChar mSelectedChar{MEGAMAN};
  UIImage* mSelectedCharImage{nullptr};
};

#endif  // MEGA_SURVIVOR_CHARSELECTION_H