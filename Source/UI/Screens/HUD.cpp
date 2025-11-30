// HUD.cpp
#include "HUD.h"
#include <algorithm>
#include <sstream>
#include "../../Actors/Player.h"
#include "../../Game.h"
#include "../../Managers/ColorPalette.h"
#include "PausedMenu.h"
#include "UIScreen.h"

constexpr float XP_MAX_SIZE_X = 455.0f;
constexpr float XP_MAX_SIZE_Y = 70.0f;
const Vector2 xpBarPos(-280.0f, 210.0f);

HUD::HUD(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName),
      mXpBar(nullptr),
      mScore(nullptr),
      mFontName(fontName),
      mDisplayedProgress(0.0f) {
  const Vector2 basePos(0.0f, 220.0f);

  float initialProgress = 0.0f;
  mXpBar = AddRect(GetXPBarOffset(initialProgress),
                   GetXPBarSize(initialProgress), 0.5f, 0.0f, 50);
  mXpBar->SetColor(ColorPalette::GetInstance().GetColorAsVec4("Orange"));

  UIImage* xp_bar =
      AddImage("../Assets/HUD/xp_bar.png", xpBarPos, 0.5f, 0.0f, 100);

  mRunTime = AddText("0:0", basePos, 0.4f, 0.0f, 40, 1024, 50);
  mRunTime->SetBackgroundColor(Vector4::Zero);
}

void HUD::Update(float deltaTime) {
  UIScreen::Update(deltaTime);

  std::stringstream ss;
  ss << static_cast<unsigned>(mGame->GetRunMinutes()) << ":"
     << static_cast<unsigned>(mGame->GetRunSeconds());

  std::string runTimeText = ss.str();

  mRunTime->SetText(runTimeText);

  const Player* player = GetGame()->GetPlayer();
  float currentXP = (float)player->GetCurrentXP();
  float maxXP = (float)player->GetMaxXP();
  float targetPct = maxXP > 0 ? currentXP / maxXP : 0.0f;

  // Smoothly interpolate displayed progress
  float lerpSpeed = 2.0f;
  mDisplayedProgress =
      Math::Lerp(mDisplayedProgress, targetPct, deltaTime * lerpSpeed);

  SetXPBar(mDisplayedProgress);
}

// Atualiza texto do score
void HUD::SetScore(int score) {
  if (!mScore)
    return;

  mScore->SetText(std::to_string(score));
}

void HUD::SetXPBar(float percentage) {
  if (!mXpBar) {
    return;
  }

  percentage = std::clamp(percentage, 0.0f, 1.0f);

  // Update the visual fill rect
  mXpBar->SetSize(GetXPBarSize(percentage));
  mXpBar->SetOffset(GetXPBarOffset(percentage));
}

Vector2 HUD::GetXPBarSize(float bar_progress) {
  return Vector2(bar_progress * XP_MAX_SIZE_X, XP_MAX_SIZE_Y);
}

Vector2 HUD::GetXPBarOffset(float bar_progress) {
  float centerX = xpBarPos.x + (bar_progress * XP_MAX_SIZE_X / 4.0f) - 100.0f;
  return Vector2(centerX, xpBarPos.y);
}

void HUD::HandleKeyPress(int key) {
  if (key == SDLK_ESCAPE) {
    new PausedMenu(GetGame(), mFontName);
    auto game = GetGame();
    if (game) {
      game->SetPaused(true);
    }
  }
}