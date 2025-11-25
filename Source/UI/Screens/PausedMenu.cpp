// PauseMenu.cpp
#include "PausedMenu.h"
#include "../../Game.h"
#include "../UIButton.h"
#include "../UIRect.h"

PauseMenu::PauseMenu(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  // Fundo semi-transparente
  auto* bg =
      AddRect(Vector2(0.0f, 0.0f), Vector2(1024.0f, 768.0f), 1.0f, 0.0f, 50);
  bg->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.5f));

  // Título
  AddText("PAUSE", Vector2(0.0f, 200.0f), 1.0f, 0.0f, 64, 1024, 100);

  // Botão "Continuar"
  AddButton(
      "Continuar",
      [this]() {
        // só fecha o menu de pausa
        mGame->SetPaused(false);
        Close();
      },
      Vector2(0.0f, 50.0f), 1.0f, 0.0f, 40, 1024, 101);

  // Botão "Voltar ao menu"
  AddButton(
      "Voltar ao menu",
      [this]() {
        mGame->SetPaused(false);
        mGame->SetScene(GameScene::MainMenu);
      },
      Vector2(0.0f, -50.0f), 1.0f, 0.0f, 40, 1024, 101);
}

void PauseMenu::HandleKeyPress(int key) {
  if (mButtons.empty())
    return;

  int oldIndex = mSelectedButtonIndex;

  switch (key) {
    case SDLK_UP:
      if (mSelectedButtonIndex > 0)
        mSelectedButtonIndex--;
      else
        mSelectedButtonIndex = static_cast<int>(mButtons.size()) - 1;
      break;

    case SDLK_DOWN:
      if (mSelectedButtonIndex < static_cast<int>(mButtons.size()) - 1)
        mSelectedButtonIndex++;
      else
        mSelectedButtonIndex = 0;
      break;

    case SDLK_RETURN:
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
    }

    if (mSelectedButtonIndex >= 0 &&
        mSelectedButtonIndex < static_cast<int>(mButtons.size())) {
      mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    }
  }
}
