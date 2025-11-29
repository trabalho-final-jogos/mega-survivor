// HUD.cpp
#include "HUD.h"
#include <algorithm>
#include <sstream>
#include "../../Game.h"
#include "PausedMenu.h"
#include "UIScreen.h"

HUD::HUD(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName), mXpBar(nullptr), mScore(nullptr) {
  // Canto superior esquerdo (ajuste fino se quiser)
  const Vector2 basePos(0.0f, 220.0f);

  // Vamos alinhar os 3 segmentos na MESMA coordenada Y da barra
  const float y = basePos.y;

  // Barra de fundo (sempre visível) – âncora da barra
  UIImage* bar = AddImage("../Assets/HUD/xp_bar.png", Vector2(-280.0f, 210.0f),
                          0.5f, 0.0f, 110);

  mRunTime = AddText("0:0", basePos, 0.4f, 0.0f, 40, 1024, 50);
  mRunTime->SetBackgroundColor(Vector4::Zero);
}

void HUD::SetXPBar(float percentage) {
  if (!mXpBar) {
    return;
  }

  // Garante que o percentual está entre 0.0 e 1.0
  percentage = std::clamp(percentage, 0.0f, 1.0f);
}

void HUD::Update(float deltaTime) {
  UIScreen::Update(deltaTime);

  std::stringstream ss;
  ss << static_cast<unsigned>(mGame->GetRunMinutes()) << ":"
     << static_cast<unsigned>(mGame->GetRunSeconds());

  std::string runTimeText = ss.str();

  mRunTime->SetText(runTimeText);
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
    new PausedMenu(GetGame(), "../Assets/Fonts/Arial.ttf");
    auto game = GetGame();
    if (game) {
      game->SetPaused(true);
    }
    // O construtor de UIScreen já faz PushUI(this) no Game
  }
}