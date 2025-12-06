#include "LevelUp.h"
#include <algorithm>
#include <random>
#include "../../Actors/Player.h"
#include "../../Game.h"

// Define available run upgrades locally since RunUpgrade.h is missing
// and we are replacing the system anyway.
const std::vector<RunUpgrade> AVAILABLE_RUN_UPGRADES = {
    {Stats::Speed, "Increase Speed"},  {Stats::Damage, "Increase Damage"},
    {Stats::Area, "Increase Area"},    {Stats::Projectiles, "Extra Projectile"},
    {Stats::Regen, "Health Regen"},    {Stats::Lucky, "Increase Luck"},
    {Stats::Health, "Increase Max HP"}};

LevelUp::LevelUp(Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  // Pause the game when this screen is created
  // mGame->SetPaused(true); // Assuming Player or Game handles pausing before
  // creating this screen, or we do it here. Game::SetPaused logic usually
  // creates PausedMenu, but here we just want to stop actors.
  mGame->SetPaused(true);

  AddText("Level Up!", Vector2(0.0f, 100.0f), 0.5f, 0.0f, 64, 1024, 100);

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
  Vector2 pos(0.0f, 0.0f);
  for (const auto& upgrade : mOptions) {
    UIButton* btn = AddButton(
        upgrade.description,
        [this, upgrade]() { this->SelectUpgrade(upgrade.type); }, pos, 0.5f,
        0.0f, 32, 1024, 102);
    btn->SetBackgroundColor(
        ColorPalette::GetInstance().GetColorAsVec4("FX_Glow"));
    pos.y -= 75.0f;  // Spacing
  }

  mSelectedButtonIndex = 0;
  if (!mButtons.empty()) {
    mButtons[0]->SetHighlighted(true);
    mButtons[0]->SetSelected(true);
  }
}

LevelUp::~LevelUp() {
  // Unpause when screen is destroyed
  mGame->SetPaused(false);
}

void LevelUp::HandleKeyPress(int key) {
  if (mButtons.empty())
    return;

  int oldIndex = mSelectedButtonIndex;

  switch (key) {
    case SDLK_UP:
    case SDLK_w:
      if (mSelectedButtonIndex > 0)
        mSelectedButtonIndex--;
      else
        mSelectedButtonIndex = static_cast<int>(mButtons.size()) - 1;
      break;

    case SDLK_DOWN:
    case SDLK_s:
      if (mSelectedButtonIndex < static_cast<int>(mButtons.size()) - 1)
        mSelectedButtonIndex++;
      else
        mSelectedButtonIndex = 0;
      break;

    case SDLK_RETURN:
    case SDLK_KP_ENTER:
      if (mSelectedButtonIndex >= 0 &&
          mSelectedButtonIndex < static_cast<int>(mButtons.size())) {
        mButtons[mSelectedButtonIndex]->OnClick();
      }
      break;

    default:
      break;
  }

  if (oldIndex != mSelectedButtonIndex) {
    if (oldIndex >= 0 && oldIndex < static_cast<int>(mButtons.size())) {
      mButtons[oldIndex]->SetHighlighted(false);
      mButtons[oldIndex]->SetSelected(false);
    }

    if (mSelectedButtonIndex >= 0 &&
        mSelectedButtonIndex < static_cast<int>(mButtons.size())) {
      mButtons[mSelectedButtonIndex]->SetHighlighted(true);
      mButtons[mSelectedButtonIndex]->SetSelected(true);
    }
  }
}

void LevelUp::SelectUpgrade(Stats type) {
  Player* player = mGame->GetPlayer();
  if (player) {
    // Determine amount
    float amount = 1.0f;
    switch (type) {
      case Stats::Speed:
      case Stats::Damage:
      case Stats::Area:
        amount = ADDITIONAL_MULTIPLIER_PER_UPGRADE;  // 0.2f (defined in
                                                     // UpgradeComponent.h)
        break;
      case Stats::Projectiles:
      case Stats::Regen:
      case Stats::Lucky:
      case Stats::Health:
        amount = static_cast<float>(ADDITIONAL_AMOUNT_PER_UPGRADE);  // 5.0f
        break;
      default:
        amount = 1.0f;
    }
    player->ApplyRunUpgrade(type, amount);
  }
  Close();
}
