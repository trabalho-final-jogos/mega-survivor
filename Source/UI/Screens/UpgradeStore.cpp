#include <algorithm>
#include "../../Game.h"
#include "../../Managers/ColorPalette.h"
#include "../../Components/Upgrades/UpgradeComponent.h"
#include "UpgradeStore.h"

UpgradeStore::UpgradeStore(Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  // Access persistent data from Game
  auto upgrades = mGame->GetPersistentUpgrades();
  // We can't use CanAfford/Purchase from UpgradeManager anymore.
  // We implement them here or in Game/UpgradeComponent.

  AddImage("../Assets/Levels/UpgradeStore/background.png", Vector2(0.0f, 0.0f),
           0.35f, 0.0f, 50);

  AddText("Upgrades", Vector2(0.0f, 200.0f), 0.5f, 0.0f, 64, 1024, 100);

  mCurrencyText = AddText("Currency: " + std::to_string(mGame->GetCurrency()),
                          Vector2(250.0f, 150.0f), 0.4f, 0.0f, 32, 512, 100);

  UIButton* but[statButtons.size() + 1]{nullptr};

  for (size_t i = 0; i < statButtons.size(); ++i) {
    int col = i % cols;
    int row = i / cols;
    Vector2 pos(startX + col * spacingX, startY - row * spacingY);

    Stats type = statButtons[i].second;
    int level = upgrades->GetBaseLevel(type);
    int cost = upgrades->GetUpgradeCost(type);

    std::string buttonText =
        statButtons[i].first + " (Lvl " +
        std::to_string(level) +
        ") - Cost: " +
        std::to_string(cost);

    but[i] = AddButton(
        buttonText,
        [this, type, upgrades]() {
          int currentCost = upgrades->GetUpgradeCost(type);
          if (mGame->GetCurrency() >= currentCost) {
             mGame->AddCurrency(-currentCost);

             // Determine amount. For now, use 1.0f or constants.
             // If we want to replicate 0.2f multiplier or +5 amount.
             float amount = 0.0f;
             switch(type) {
                 case Stats::Speed:
                 case Stats::Damage:
                 case Stats::Area:
                     amount = ADDITIONAL_MULTIPLIER_PER_UPGRADE; // 0.2f
                     break;
                 case Stats::Projectiles:
                 case Stats::Regen:
                 case Stats::Lucky:
                 case Stats::Health:
                     amount = static_cast<float>(ADDITIONAL_AMOUNT_PER_UPGRADE); // 5.0f
                     break;
                 default:
                     amount = 1.0f;
             }

             upgrades->UpgradeBaseStat(type, amount);
          }
        },
        pos, 0.4f, 0.0f, 32, 256, 102);

    if (mGame->GetCurrency() < cost) {
      auto _color = ColorPalette::GetInstance().GetColorAsVec4("Red_bright");
      but[i]->SetBackgroundColor(_color);
    } else {
      auto _color = ColorPalette::GetInstance().GetColorAsVec4("Lime_green");
      but[i]->SetBackgroundColor(_color);
    }

    mUpgradeButtons[i] = but[i];

    mSelectedButtonIndex = 0;
  }

  UIButton* escape_but = AddButton(
      "Go back", [this]() { mGame->SetScene(GameScene::MainMenu); },
      Vector2(200.0f, -150.0f), 0.5f, 0.0f, 32, 128, 102);

  escape_but->SetTextColor(
      ColorPalette::GetInstance().GetColorAsVec4("Yellow_bright"));

  if (!mButtons.empty()) {
    mButtons[0]->SetHighlighted(true);
    mButtons[0]->SetSelected(true);
  }
}

void UpgradeStore::Update(float deltaTime) {
  UIScreen::Update(deltaTime);

  auto upgrades = mGame->GetPersistentUpgrades();

  // Refresh currency
  if (mCurrencyText) {
    mCurrencyText->SetText("Currency: " + std::to_string(mGame->GetCurrency()));
  }

  for (size_t i = 0; i < mUpgradeButtons.size() && mUpgradeButtons[i]; ++i) {
    Stats type = statButtons[i].second;
    int level = upgrades->GetBaseLevel(type);
    int cost = upgrades->GetUpgradeCost(type);

    std::string newText =
        statButtons[i].first + " (Lvl " +
        std::to_string(level) +
        ") - Cost: " +
        std::to_string(cost);
    mUpgradeButtons[i]->SetText(newText);

    if (mGame->GetCurrency() < cost) {
      auto _color = ColorPalette::GetInstance().GetColorAsVec4("Red_bright");
      mUpgradeButtons[i]->SetBackgroundColor(_color);
    } else {
      auto _color = ColorPalette::GetInstance().GetColorAsVec4("Lime_green");
      mUpgradeButtons[i]->SetBackgroundColor(_color);
    }
  }
}

void UpgradeStore::HandleKeyPress(int key) {
  if (mButtons.empty()) {
    return;
  }

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
      if (mSelectedButtonIndex < static_cast<int>(mButtons.size()) + cols)
        mSelectedButtonIndex += cols;
      else
        mSelectedButtonIndex = static_cast<int>(statButtons.size());
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

  mSelectedButtonIndex = std::clamp(mSelectedButtonIndex, 0,
                                    static_cast<int>(mButtons.size()) - 1);

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