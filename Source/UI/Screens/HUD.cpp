// HUD.cpp
#include "HUD.h"
#include "../../Game.h"
#include "PausedMenu.h"
#include "UIScreen.h"

HUD::HUD(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName),
      mHealth1(nullptr),
      mHealth2(nullptr),
      mHealth3(nullptr),
      mScore(nullptr) {
  // Canto superior esquerdo (ajuste fino se quiser)
  const Vector2 basePos(-220.0f, 350.0f);

  // Vamos alinhar os 3 segmentos na MESMA coordenada Y da barra
  const float y = basePos.y;

  // Offsets em X relativos ao início da barra
  mHealth1 =
      AddImage("../Assets/HUD/ShieldRed.png", Vector2(basePos.x, y), 0.75f);

  mHealth2 = AddImage("../Assets/HUD/ShieldOrange.png",
                      Vector2(basePos.x + 50.0f, y), 0.75f);

  mHealth3 =
      AddImage("../Assets/HUD/ShieldBlue.png", Vector2(basePos.x, y), 0.75f);

  // Barra de fundo (sempre visível) – âncora da barra
  UIImage* bar =
      AddImage("../Assets/HUD/ShieldBar.png", basePos, 0.75f, 0.0f, 110);

  // Texto "score:" um pouco abaixo da barra
  AddText("score: ", basePos + Vector2(0.0f, -40.0f),  // -40 -> desce na tela
          1.0f);

  // Contador de pontos ao lado do "score:"
  mScore = AddText("0", basePos + Vector2(100.0f, -40.0f), 1.0f);
}

// Atualiza barra de vida conforme health (0..3)
void HUD::SetHealth(int health) {
  if (mHealth1)
    mHealth1->SetIsVisible(health >= 1);
  if (mHealth2)
    mHealth2->SetIsVisible(health >= 2);
  if (mHealth3)
    mHealth3->SetIsVisible(health >= 3);
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