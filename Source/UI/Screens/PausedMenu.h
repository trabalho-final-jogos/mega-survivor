// PauseMenu.h
#pragma once

#include "UIScreen.h"

class PausedMenu : public UIScreen {
 public:
  PausedMenu(class Game* game, const std::string& fontName);

  void HandleKeyPress(int key) override;
};
