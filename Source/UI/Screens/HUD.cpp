#include "HUD.h"
#include <algorithm>
#include <sstream>
#include "../../Actors/Player.h"
#include "../../Game.h"
#include "../../Managers/ColorPalette.h"
#include "PausedMenu.h"
#include "UIScreen.h"

// RESTORED ORIGINAL CONSTANTS
constexpr float XP_MAX_SIZE_X = 455.0f;
constexpr float XP_MAX_SIZE_Y = 70.0f;
const Vector2 xpBarPos(-280.0f, 210.0f);

constexpr float HP_BAR_WIDTH =
    60.0f;  // Adjusted to match your logic inside Update
constexpr float HP_BAR_HEIGHT = 8.0f;
constexpr float HP_BAR_OFFSET_Y = 40.0f;

HUD::HUD(class Game* game, const std::string& fontName)
    : UIScreen(game, fontName),
      mXpBar(nullptr),
      mScore(nullptr),
      mFontName(fontName),
      mDisplayedProgress(0.0f),
      mLastPlayerHP(0) {
  const Vector2 basePos(0.0f, 220.0f);

  // Health Bar setup
  mHealthBarBg = AddRect(Vector2::Zero, Vector2(60.0f, 10.0f), 1.0f, 0.0f, 150);
  mHealthBarBg->SetColor(ColorPalette::GetInstance().GetColorAsVec4("Smoke"));

  mHealthBar = AddRect(Vector2(0.0f, HP_BAR_OFFSET_Y), Vector2(60.0f, 10.0f),
                       1.0f, 0.0f, 151);
  mHealthBar->SetColor(
      ColorPalette::GetInstance().GetColorAsVec4("Lime_green"));

  // XP Bar setup (RESTORED ORIGINAL)
  float initialProgress = 0.0f;
  mXpBar = AddRect(GetXPBarOffset(initialProgress),
                   GetXPBarSize(initialProgress), 0.5f, 0.0f, 50);
  mXpBar->SetColor(ColorPalette::GetInstance().GetColorAsVec4("Orange"));

  UIImage* xp_bar =
      AddImage("../Assets/HUD/xp_bar.png", xpBarPos, 0.5f, 0.0f, 100);

  mRunTime = AddText("0:0", basePos, 0.4f, 0.0f, 40, 1024, 50);
  mRunTime->SetBackgroundColor(Vector4::Zero);

  mOverlay =
      AddRect(Vector2::Zero, Vector2(Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT),
              1.0f, 0.0f, 200);  // Draw order high
  mOverlay->SetColor(Vector4(1.0f, 0.0f, 0.0f, 0.0f));
}

void HUD::ShowFlash() {
  mFlashTimer = 0.5f;
}

void HUD::Update(float deltaTime) {
  UIScreen::Update(deltaTime);
  const Player* player = GetGame()->GetPlayer();

  if (!player)
    return;

  auto hp = player->GetCurrentHP();

  // Initialize tracking variable if this is the first frame
  if (mLastPlayerHP == 0 && hp > 0)
    mLastPlayerHP = hp;

  if (hp != mLastPlayerHP) {
    mFlashColor =
        (hp > mLastPlayerHP)
            ? ColorPalette::GetInstance().GetColorAsVec3("Lime_green")
            : ColorPalette::GetInstance().GetColorAsVec3("Red_bright");
    ShowFlash();
    mLastPlayerHP = hp;
  }

  if (mFlashTimer > 0.0f) {
    mFlashTimer -= deltaTime;
    float alpha = Math::Clamp(mFlashTimer / 0.5f, 0.0f, 0.5f);
    mOverlay->SetColor(Vector4(mFlashColor, alpha));
  } else {
    mOverlay->SetColor(Vector4(mFlashColor, 0.0f));
  }

  std::stringstream ss;
  ss << static_cast<unsigned>(mGame->GetRunMinutes()) << ":"
     << static_cast<unsigned>(mGame->GetRunSeconds());

  std::string runTimeText = ss.str();
  mRunTime->SetText(runTimeText);

  float currentXP = (float)player->GetCurrentXP();
  float maxXP = (float)player->GetMaxXP();
  float targetPct = maxXP > 0 ? currentXP / maxXP : 0.0f;

  float lerpSpeed = 2.0f;
  mDisplayedProgress =
      Math::Lerp(mDisplayedProgress, targetPct, deltaTime * lerpSpeed);

  SetXPBar(mDisplayedProgress);

  // --- HEALTH BAR LOGIC FIXED HERE ---
  UpdateHealthBar(player);
}

void HUD::UpdateHealthBar(const Player* player) {
  float alpha = mGame->IsPaused() ? 0.1f : 1.0f;

  Vector2 playerPos = player->GetPosition();
  Vector2 cameraPos = GetGame()->GetCameraPos();
  Vector2 relPos = playerPos - cameraPos;

  float screenX = relPos.x - Game::VIRTUAL_WIDTH / 2.0f;
  float screenY = Game::VIRTUAL_HEIGHT / 2.0f - relPos.y;

  float barY = screenY + HP_BAR_OFFSET_Y;
  float barX = screenX;

  // Original dimensions from your code
  float barWidth = 60.0f;
  float barHeight = 8.0f;

  // Background update
  mHealthBarBg->SetOffset(Vector2(barX, barY));
  mHealthBarBg->SetSize(Vector2(barWidth + 2.0f, barHeight + 2.0f));

  float currentHP = static_cast<float>(player->GetCurrentHP());
  float maxHP = static_cast<float>(player->GetMaxHP());
  float hpPercent =
      (maxHP > 0.0f) ? Math::Clamp(currentHP / maxHP, 0.0f, 1.0f) : 0.0f;

  float filledWidth = barWidth * hpPercent;

  // Left-alignment math: CenterX = LeftEdge + (NewWidth / 2)
  float filledCenterX = (barX - barWidth / 2.0f) + (filledWidth / 2.0f);

  Vector4 bgColor = ColorPalette::GetInstance().GetColorAsVec4("Smoke");
  bgColor.w = alpha;  // Set Alpha
  mHealthBarBg->SetColor(bgColor);

  Vector4 fgColor = ColorPalette::GetInstance().GetColorAsVec4("Lime_green");
  fgColor.w = alpha;  // Set Alpha
  mHealthBar->SetColor(fgColor);

  mHealthBar->SetOffset(Vector2(filledCenterX, barY));
  mHealthBar->SetSize(Vector2(filledWidth, barHeight));
}

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