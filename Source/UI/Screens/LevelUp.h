#pragma once
#include <string>
#include <vector>
#include "../../Managers/UpgradeManager.h"
#include "UIScreen.h"

class LevelUp : public UIScreen {
 public:
  LevelUp(class Game* game, const std::string& fontName);
  ~LevelUp();

  void HandleKeyPress(int key) override;

 private:
  std::vector<StatType> mOptions;
  void OnUpgradeSelected(StatType type);
};
