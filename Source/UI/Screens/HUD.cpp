// HUD.cpp
#include "HUD.h"
#include <algorithm>
#include "../../Game.h"
#include "PausedMenu.h"
#include "UIScreen.h"

HUD::HUD(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName), mXpBar(nullptr), mScore(nullptr) {
  // Canto superior esquerdo (ajuste fino se quiser)
  const Vector2 basePos(-220.0f, 350.0f);

  // Vamos alinhar os 3 segmentos na MESMA coordenada Y da barra
  const float y = basePos.y;

  // Barra de fundo (sempre visível) – âncora da barra
  UIImage* bar =
      AddImage("../Assets/HUD/ShieldBar.png", basePos, 0.75f, 0.0f, 110);

  // Texto "score:" um pouco abaixo da barra
  AddText("score: ", basePos + Vector2(0.0f, -40.0f),  // -40 -> desce na tela
          1.0f);

  // Contador de pontos ao lado do "score:"
  mScore = AddText("0", basePos + Vector2(100.0f, -40.0f), 1.0f);
}

void HUD::SetXPBar(float percentage) {
  if (!mXpBar)
    return;

  // Garante que o percentual está entre 0.0 e 1.0
  percentage = std::clamp(percentage, 0.0f, 1.0f);
}

// Atualiza texto do score
void HUD::SetScore(int score) {
  if (!mScore)
    return;

  mScore->SetText(std::to_string(score));
}

void HUD::HandleKeyPress(int key) {
  if (key == SDLK_ESCAPE) {
    // Abre o menu de pausa por cima do HUD
    new PauseMenu(GetGame(), "../Assets/Fonts/Arial.ttf");
    auto game = GetGame();
    if (game) {
      game->SetPaused(true);
    }
    // O construtor de UIScreen já faz PushUI(this) no Game
  }
}