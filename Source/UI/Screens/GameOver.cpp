// GameOver.cpp
#include "GameOver.h"
#include "../../Game.h"
#include "../../Managers/ColorPalette.h"
#include "../UIButton.h"

GameOver::GameOver(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  AddImage("../Assets/Levels/GameOver/game_over.png", Vector2(0.0f, 0.0f),
           0.35f, 0.0f, 50);

  Vector4 textColors =
      ColorPalette::GetInstance().GetColorAsVec4("Yellow_bright");

  UIButton* mainMenuButton = AddButton(
      "Main Menu",
      [this]() {
        // Return to main menu
        GetGame()->SetScene(GameScene::MainMenu);
      },
      Vector2(0.0f, 50.0f), 1.0f);

  // Configure styling
  mainMenuButton->SetBackgroundColor(
      Vector4(0.01f, 0.01f, 1.0f, Game::UNSELECTED_OPACITY));
  mainMenuButton->SetTextColor(textColors);

  // Highlight by default since it is the only button
  mSelectedButtonIndex = 0;
  if (!mButtons.empty()) {
    mButtons[0]->SetHighlighted(true);
    mButtons[0]->SetSelected(true);
    mButtons[0]->SetOpacity(Game::SELECTED_OPACITY);
  }
}

void GameOver::HandleKeyPress(int key) {
  if (mButtons.empty()) {
    return;
  }

  if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
    if (mSelectedButtonIndex >= 0 &&
        mSelectedButtonIndex < static_cast<int>(mButtons.size())) {
      mButtons[mSelectedButtonIndex]->OnClick();
    }
  }
}
