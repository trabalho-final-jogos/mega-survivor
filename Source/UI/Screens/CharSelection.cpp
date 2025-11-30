//
// Created by daniel on 29/11/2025.
//

#include "CharSelection.h"

#include "../../Actors/Player.h"
#include "../../Game.h"
#include "MainMenu.h"

CharSelection::CharSelection(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  AddText("Character Selection", Vector2(0.0f, 150.0f), 0.7f);
  AddButton(
      "Megaman",
      [this]() {
        mGame->mChar = PlayerChar::MEGAMAN;
        mGame->SetScene(GameScene::Level1);
      },
      Vector2(-150.0f, 100.0f), Game::UNSELECTED_OPACITY);

  AddButton(
      "Protoman",
      [this]() {
        mGame->mChar = PlayerChar::PROTOMAN;
        mGame->SetScene(GameScene::Level1);
      },
      Vector2(-150.0f, 50.0f), Game::UNSELECTED_OPACITY);

  AddButton(
      "Bass",
      [this]() {
        mGame->mChar = PlayerChar::BASS;
        mGame->SetScene(GameScene::Level1);
      },
      Vector2(-150.0f, 0.0f), Game::UNSELECTED_OPACITY);

  AddButton(
      "Back", [this]() { mGame->SetScene(GameScene::MainMenu); },
      Vector2(0.0f, -150.0f), Game::UNSELECTED_OPACITY);

  mSelectedButtonIndex = 0;
  if (!mButtons.empty()) {
    mButtons[0]->SetHighlighted(true);
    mButtons[0]->SetSelected(true);
    mButtons[0]->SetOpacity(Game::SELECTED_OPACITY);
  }
}

void CharSelection::HandleKeyPress(int key) {
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
    if (oldIndex >= 0 && oldIndex < static_cast<int>(mButtons.size())) {
      mButtons[oldIndex]->SetHighlighted(false);
      mButtons[oldIndex]->SetSelected(false);
      mButtons[oldIndex]->SetOpacity(Game::UNSELECTED_OPACITY);
    }
    if (mSelectedButtonIndex >= 0 &&
        mSelectedButtonIndex < static_cast<int>(mButtons.size())) {
      mButtons[mSelectedButtonIndex]->SetHighlighted(true);
      mButtons[mSelectedButtonIndex]->SetSelected(true);
      mButtons[mSelectedButtonIndex]->SetOpacity(Game::SELECTED_OPACITY);
    }
  }
}
