

#pragma once
#include "UIScreen.h"

class Level1 : public UIScreen {
 public:
  Level1(class Game* game, const std::string& fontName);
  ~Level1() override = default;
};
