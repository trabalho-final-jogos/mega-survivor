#pragma once
#include "UIScreen.h"

const int cols = 3;
const int rows = 4;
const float startX = -350.0f;
const float startY = 50.0f;
const float spacingX = 250.0f;
const float spacingY = 70.0f;

class MetaProg : public UIScreen {
 public:
  MetaProg(class Game* game, const std::string& fontName);
};
