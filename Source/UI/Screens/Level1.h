

#pragma once
#include "../../Actors/Player.h"
#include "HUD.h"
#include "UIScreen.h"

class Level1 : public UIScreen {
 public:
  Level1(class Game* game, const std::string& fontName);
  ~Level1() override = default;
  void Update(float deltaTime) override;

 private:
  uint8_t mRunSeconds;
  uint8_t mRunMinutes;
  std::string mFontName;

  HUD* mHUD;
  CharInfo mCharInfo;
};
