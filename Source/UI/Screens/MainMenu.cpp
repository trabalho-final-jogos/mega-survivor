//
// Created by Lucas N. Ferreira on 06/11/25.
//

#include "MainMenu.h"
#include "../../Game.h"
#include "../../Managers/ColorPalette.h"
#include "../UIButton.h"
#include "SDL.h"

MainMenu::MainMenu(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  AddImage("../Assets/Levels/MainMenu/main_menu.png", Vector2(0.0f, 0.0f),
           0.35f, 0.0f, 50);

  Vector4 textColors =
      ColorPalette::GetInstance().GetColorAsVec4("Yellow_bright");

  // Seleção de personagem

  UIButton* startButton = AddButton(
      "New game",
      [this]() {
        Close();                             // Fecha a tela de menu
        mGame->SetScene(GameScene::CharSelect);  // Inicia o jogo
      },
      Vector2(0.0f, -50.0f),  // Posição centralizada
      0.5f, 0.0f, 40, 1024, 101);

  // Configura cores: fundo azul, texto branco
  startButton->SetBackgroundColor(
      Vector4(0.01f, 0.01f, 1.0f, Game::UNSELECTED_OPACITY));
  startButton->SetTextColor(textColors);

  UIButton* upgradeStoreButton = AddButton(
      "Upgrades",
      [this]() {
        Close();                                   // Fecha a tela de menu
        mGame->SetScene(GameScene::UpgradeStore);  // Inicia o jogo
      },
      Vector2(0.0f, -100.0f),  // Posição centralizada
      0.5f, 0.0f, 40, 1024, 101);

  upgradeStoreButton->SetBackgroundColor(
      Vector4(0.01f, 0.01f, 1.0f, Game::UNSELECTED_OPACITY));
  upgradeStoreButton->SetTextColor(textColors);

  // Cria e configura o botão "Fechar Jogo"
  UIButton* quitButton = AddButton(
      "Exit",
      [this]() {
        mGame->Quit();  // Fecha o jogo
      },
      Vector2(0.0f, -200.0f),  // Posição abaixo do primeiro botão
      0.5f, 0.0f, 40, 1024, 101);

  quitButton->SetBackgroundColor(
      Vector4(0.01f, 0.01f, 1.0f, Game::UNSELECTED_OPACITY));
  quitButton->SetTextColor(textColors);

  mSelectedButtonIndex = 0;
  if (!mButtons.empty()) {
    mButtons[0]->SetHighlighted(true);
    mButtons[0]->SetSelected(true);
    mButtons[0]->SetOpacity(Game::SELECTED_OPACITY);
  }
}

void MainMenu::HandleKeyPress(int key) {
  if (mButtons.empty())
    return;

  int oldIndex = mSelectedButtonIndex;
  float oldOpacity = mButtons[mSelectedButtonIndex]->GetOpacity();

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
