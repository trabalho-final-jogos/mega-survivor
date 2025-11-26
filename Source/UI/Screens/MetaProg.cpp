#include "MetaProg.h"
#include "../../Game.h"
#include "../../Managers/ColorPalette.h"
#include "../../Managers/UpgradeManager.h"

MetaProg::MetaProg(Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  auto& mgr = UpgradeManager::GetInstance();

  AddText("Upgrades", Vector2(0.0f, 150.0f), 0.5f, 0.0f, 64, 1024, 100);

  mCurrencyText = AddText("Currency: " + std::to_string(mgr.GetCurrency()),
                          Vector2(200.0f, 100.0f), 0.3f, 0.0f, 32, 512, 100);

  UIButton* but[7];

  for (size_t i = 0; i < statButtons.size(); ++i) {
    int col = i % cols;
    int row = i / cols;
    Vector2 pos(startX + col * spacingX, startY - row * spacingY);

    std::string buttonText =
        statButtons[i].first + " (Lvl " +
        std::to_string(mgr.GetUpgradeLevel(statButtons[i].second)) +
        ") - Cost: " +
        std::to_string(mgr.GetUpgradeCost(statButtons[i].second));

    but[i] = AddButton(
        buttonText,
        [&mgr, type = statButtons[i].second]() {
          if (mgr.CanAfford(type)) {
            mgr.Purchase(type);
          }
        },
        pos, 0.3f, 0.0f, 32, 256, 102);

    if (mgr.GetCurrency() < mgr.GetUpgradeCost(statButtons[i].second)) {
      auto _color = ColorPalette::GetInstance().GetColorAsVec3("Red_light");
      but[i]->SetBackgroundColor(_color);  // Red tint
    } else {
      but[i]->SetBackgroundColor(Vector3(0.2f, 0.8f, 0.2f));  // Green tint
    }

    mUpgradeButtons[i] = but[i];

    mSelectedButtonIndex = 0;
    if (!mButtons.empty()) {
      mButtons[0]->SetHighlighted(true);
    }
  }
}

void MetaProg::Update(float deltaTime) {
  UIScreen::Update(deltaTime);  // Call base

  auto& mgr = UpgradeManager::GetInstance();

  // Refresh currency
  if (mCurrencyText) {
    mCurrencyText->SetText("Currency: " + std::to_string(mgr.GetCurrency()));
  }

  // Refresh 7 upgrade buttons
  for (size_t i = 0; i < mUpgradeButtons.size() && mUpgradeButtons[i]; ++i) {
    // Or recreate buttonText as in ctor:
    std::string newText =
        statButtons[i].first + " (Lvl " +  // Assume statButtons local or member
        std::to_string(
            static_cast<int>(mgr.GetUpgradeLevel(statButtons[i].second))) +
        ") - Cost: " +
        std::to_string(mgr.GetUpgradeCost(statButtons[i].second));
    mUpgradeButtons[i]->SetText(newText);

    if (!mgr.CanAfford(statButtons[i].second)) {
      mUpgradeButtons[i]->SetBackgroundColor(Vector3(0.8f, 0.2f, 0.2f));
    } else {
      mUpgradeButtons[i]->SetBackgroundColor(Vector3(0.2f, 0.8f, 0.2f));
    }
  }
}

void MetaProg::HandleKeyPress(int key) {
  if (mButtons.empty())
    return;

  int oldIndex = mSelectedButtonIndex;

  switch (key) {
    case SDLK_UP:
    case SDLK_w:
      if (mSelectedButtonIndex > 0)
        mSelectedButtonIndex -= cols;
      else
        mSelectedButtonIndex = static_cast<int>(mButtons.size()) - 1;
      break;

    case SDLK_DOWN:
    case SDLK_s:
      if (mSelectedButtonIndex < static_cast<int>(mButtons.size()) - 1)
        mSelectedButtonIndex += cols;
      else
        mSelectedButtonIndex = 0;
      break;

    case SDLK_RIGHT:
    case SDLK_d:
      if ((mSelectedButtonIndex + 1) % cols != 0 &&
          mSelectedButtonIndex < static_cast<int>(mButtons.size()) - 1)
        mSelectedButtonIndex++;
      break;

    case SDLK_LEFT:
    case SDLK_a:
      if (mSelectedButtonIndex % cols != 0)
        mSelectedButtonIndex--;
      break;

    case SDLK_RETURN:    // Enter works ✓
    case SDLK_KP_ENTER:  // Numpad Enter
      if (mSelectedButtonIndex >= 0 &&
          mSelectedButtonIndex < static_cast<int>(mButtons.size())) {
        mButtons[mSelectedButtonIndex]->OnClick();
      }
      break;

    case SDLK_ESCAPE:
      mGame->SetScene(GameScene::MainMenu);
      break;

    default:
      break;
  }

  // Update highlight
  if (oldIndex != mSelectedButtonIndex) {
    if (oldIndex >= 0 && oldIndex < static_cast<int>(mButtons.size()))
      mButtons[oldIndex]->SetHighlighted(false);
    if (mSelectedButtonIndex >= 0 &&
        mSelectedButtonIndex < static_cast<int>(mButtons.size()))
      mButtons[mSelectedButtonIndex]->SetHighlighted(true);
  }
}