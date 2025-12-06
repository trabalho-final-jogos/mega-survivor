//
// Created by daniel on 29/11/2025.
//

#include "CharSelection.h"

#include "../../Actors/Player.h"
#include "../../Game.h"
#include "MainMenu.h"

CharSelection::CharSelection(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  AddImage("../Assets/Levels/CharMenu/char_bg.png", Vector2(0.0f, 0.0f), 0.35f,
           0.0f, 50);

  AddText("Character Selection", Vector2(0.0f, 200.0f), 0.7f);

  Vector2 charButtonsPos(-200.0f, 100.0f);
  Vector2 charImagePos(200.0f, 0.0f);
  float charButOffset = -75.0f;
  UIButton* but[kCharCount + 1]{nullptr};

  for (uint i = 0; i < kCharCount; i++) {
    CharInfo _char = CharacterDB::Get(static_cast<PlayerChar>(i));

    but[i] = AddButton(
        _char.charName.data(),
        [this, _char]() {
          mGame->mChar = _char.playerChar;
          mGame->SetScene(GameScene::Level1);
        },
        charButtonsPos + Vector2(0.0f, charButOffset * i),
        Game::UNSELECTED_OPACITY);

    mCharButtons[i] = but[i];
    but[i]->SetHighlighted(false);
    but[i]->SetSelected(false);
    but[i]->SetOpacity(Game::UNSELECTED_OPACITY);
  }

  mSelectedButtonIndex = 0;

  AddButton(
      "Back", [this]() { mGame->SetScene(GameScene::MainMenu); },
      Vector2(0.0f, -150.0f), Game::UNSELECTED_OPACITY);

  mSelectedCharImage = AddImage("../Assets/Sprites/Megaman/player.png",
                                charImagePos, 3.0f, 0.0f, 100);

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
      mSelectedChar = static_cast<PlayerChar>(mSelectedButtonIndex);
    }
  }
}
