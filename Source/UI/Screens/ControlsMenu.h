#pragma once
#include "UIScreen.h"

class ControlsMenu : public UIScreen {
 public:
  ControlsMenu(class Game* game,
               const std::string& fontName,
               UIScreen* parent = nullptr);
  void HandleKeyPress(int key);

 private:
  UIScreen* mParent;
};