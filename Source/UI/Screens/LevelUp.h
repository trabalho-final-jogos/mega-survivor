#pragma once
#include <string>
#include <vector>
#include "../../Components/Upgrades/UpgradeComponent.h"
#include "UIScreen.h"

struct RunUpgrade {
    Stats type;
    std::string description;
};

class LevelUp : public UIScreen {
 public:
  LevelUp(class Game* game, const std::string& fontName);
  ~LevelUp();

  void HandleKeyPress(int key) override;

 private:
  std::vector<RunUpgrade> mOptions;
  void SelectUpgrade(Stats type);
};
