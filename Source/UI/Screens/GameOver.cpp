// GameOver.cpp
#include "GameOver.h"
#include "../../Game.h"

GameOver::GameOver(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  // Texto "Game Over" centralizado
  AddText("Game Over", Vector2(0.0f, -50.0f),
          1.5f);  // escala maior

  // Botão "Press Enter" que volta para o menu
  AddButton(
      "Press Enter",
      [this]() {
        // Fecha esta tela e volta ao menu principal
        GetGame()->SetScene(GameScene::MainMenu);
      },
      Vector2(0.0f, 50.0f), 1.0f);
}

void GameOver::HandleKeyPress(int key) {
  // Deixa a UIScreen básica tratar seleção, etc.
  UIScreen::HandleKeyPress(key);

  // Quando o usuário pressiona Enter, aciona o botão selecionado
  if (key == SDLK_RETURN || key == SDLK_KP_ENTER) {
    // O UIScreen já mantém mSelectedButtonIndex; se quiser,
    // pode explicitamente acionar o callback do botão atual,
    // mas o UIButton padrão normalmente já faz isso.
    // Se precisar, você pode buscar mButtons[mSelectedButtonIndex]
    // e chamar OnClick().
  }
}
