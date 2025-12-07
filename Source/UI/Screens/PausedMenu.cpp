// PauseMenu.cpp
#include "PausedMenu.h"
#include "../../Game.h"
#include "../../Managers/ColorPalette.h"
#include "../UIButton.h"
#include "../UIRect.h"
#include "ControlsMenu.h"
#include "SDL.h"

PausedMenu::PausedMenu(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  // Fundo semi-transparente
  auto* bg =
      AddRect(Vector2(0.0f, 0.0f), Vector2(1024.0f, 768.0f), 1.0f, 0.0f, 50);
  bg->SetColor(Vector4(0.0f, 0.0f, 0.0f, 0.6f));

  // Título
  AddText("Paused", Vector2(0.0f, 100.0f), 0.5f, 0.0f, 64, 1024, 100);

  Vector4 textColors = ColorPalette::GetInstance().GetColorAsVec4("UI_Text");

  // Botão "Continuar"
  UIButton* continueButton = AddButton(
      "Continue",
      [this]() {
        // só fecha o menu de pausa
        mGame->SetPaused(false);
        Close();
      },
      Vector2(0.0f, 50.0f), 0.3f, 0.0f, 40, 1024, 101);

  continueButton->SetBackgroundColor(Vector4(0.01f, 0.01f, 1.0f, 1.0f));
  continueButton->SetTextColor(textColors);

  UIButton* controlsMenuButton = AddButton(
      "Controls",
      [this]() {
        // só fecha o menu de pausa
        new ControlsMenu(GetGame(), GAME_FONT.data(), this);
      },
      Vector2(0.0f, 0.0f), 0.3f, 0.0f, 40, 1024, 101);

  controlsMenuButton->SetBackgroundColor(Vector4(0.01f, 0.01f, 1.0f, 1.0f));
  controlsMenuButton->SetTextColor(textColors);

  // Botão "Voltar ao menu"
  UIButton* exitButton = AddButton(
      "Exit to main menu", [this]() { mGame->ResetGame(); },
      Vector2(0.0f, -50.0f), 0.3f, 0.0f, 40, 1024, 101);

  exitButton->SetBackgroundColor(Vector4(0.01f, 0.01f, 1.0f, 1.0f));
  exitButton->SetTextColor(textColors);

  mSelectedButtonIndex = 0;
  if (!mButtons.empty()) {
    mButtons[0]->SetHighlighted(true);
    mButtons[0]->SetSelected(true);
  }
}

void PausedMenu::HandleKeyPress(int key) {
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

    case SDLK_ESCAPE:
      mGame->SetPaused(false);
      Close();
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
