//
// Created by Lucas N. Ferreira on 06/11/25.
//

#include "MainMenu.h"
#include "../../Game.h"
#include "../UIButton.h"
#include "SDL.h"

MainMenu::MainMenu(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  // Adiciona a logo do jogo centralizada no topo da tela
  // Ajuste o caminho conforme necessário
  // AddImage("../Assets/Logo.png", Vector2(0.0f, 0.0f), 0.6f, 0.0f, 100);
  AddText("MEGA SURVIVORS", Vector2(0.0f, 100.0f), 0.5f, 0.0f, 64, 1024, 100);

  // Cria e configura o botão "Iniciar Jogo"
  UIButton* startButton = AddButton(
      "New game",
      [this]() {
        Close();                             // Fecha a tela de menu
        mGame->SetScene(GameScene::Level1);  // Inicia o jogo
      },
      Vector2(0.0f, -100.0f),  // Posição centralizada
      0.5f, 0.0f, 40, 1024, 101);

  // Configura cores: fundo azul, texto branco
  startButton->SetBackgroundColor(Vector3(0.01f, 0.01f, 1.0f));
  startButton->SetTextColor(Vector3(1.0f, 1.0f, 1.0f));

  // Cria e configura o botão "Fechar Jogo"
  UIButton* quitButton = AddButton(
      "Exit",
      [this]() {
        mGame->Quit();  // Fecha o jogo
      },
      Vector2(0.0f, -150.0f),  // Posição abaixo do primeiro botão
      0.5f, 0.0f, 40, 1024, 101);

  mSelectedButtonIndex = 0;
  if (!mButtons.empty()) {
    mButtons[0]->SetHighlighted(true);
  }

  // Configura cores: fundo azul, texto branco
  quitButton->SetBackgroundColor(Vector3(0.01f, 0.01f, 1.0f));
  quitButton->SetTextColor(Vector3(1.0f, 1.0f, 1.0f));
}

void MainMenu::HandleKeyPress(int key) {
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
