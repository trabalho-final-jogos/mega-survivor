#include "LevelUp.h"
#include <algorithm>
#include <random>
#include "../../Actors/Player.h"
#include "../../Game.h"
#include "../../Managers/RunUpgrade.h"

LevelUp::LevelUp(Game* game, const std::string& title) : UIScreen(game) {
  // Pause the game when this screen is created (redundant if done in Player,
  // but safe) mGame->SetPaused(true);

  // Select 3 random upgrades
  std::vector<RunUpgrade> allUpgrades = AVAILABLE_RUN_UPGRADES;

  // Simple random shuffle
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(allUpgrades.begin(), allUpgrades.end(), g);

  // Take top 3 or less if not enough
  int count = std::min((int)allUpgrades.size(), 3);
  for (int i = 0; i < count; ++i) {
    mOptions.push_back(allUpgrades[i]);
  }

  // Create UI Buttons
  Vector2 pos(0.0f, -100.0f);
  for (const auto& upgrade : mOptions) {
    UIButton* btn =
        new UIButton(mGame, upgrade.description, mFont,
                     [this, upgrade]() { this->SelectUpgrade(upgrade.type); });
    btn->SetPosition(pos);
    pos.y += 100.0f;  // Spacing
  }
}

LevelUp::~LevelUp() {
  // Unpause when screen is destroyed
  mGame->SetPaused(false);
}

void LevelUp::HandleKeyPress(int key) {
  // ESC to close without selecting? Maybe not allowed in Level Up.
  // For now, allow it for debugging or force selection.
  // if (key == SDL_SCANCODE_ESCAPE) {
  //   Close();
  // }
  UIScreen::HandleKeyPress(key);
}

void LevelUp::SelectUpgrade(RunUpgradeType type) {
  Player* player = mGame->GetPlayer();
  if (player) {
    player->ApplyRunUpgrade(type);
  }
  Close();
}
