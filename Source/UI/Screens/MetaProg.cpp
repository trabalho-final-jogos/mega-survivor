#include "MetaProg.h"
#include "../../Actors/Player.h"
#include "../../Game.h"

MetaProg::MetaProg(Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  // Adiciona a logo do jogo centralizada no topo da tela
  // Ajuste o caminho conforme necessário
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