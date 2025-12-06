#include "Level1.h"
#include "../../Actors/Player.h"
#include "../../Actors/Spawner.h"
#include "../../Game.h"
#include "GameOver.h"

Level1::Level1(Game* game, const std::string& fontName)
    : UIScreen(game, fontName), mFontName(fontName) {
  int** level = mGame->LoadLevel("../Assets/Levels/Level1/chao.csv", 60, 60);
  mGame->SetLevelData(level);

  SDL_Log("Level data loaded for Level 1-1");

  mGame->BuildLevel(level, 60, 60);

  SDL_Log("Level 1-1 built successfully");

  mHUD = new HUD(game, fontName);

  mHUD->SetXPBar(0.0f);

  if (!mGame->GetPlayer()) {
    Player* player = new Player(mGame, mGame->GetCharInfo());
    player->SetPosition(
        Vector2(Game::WINDOW_WIDTH / 2, Game::WINDOW_HEIGHT / 2));
    mGame->SetPlayer(player);
  }
  mGame->StartClock();
  new Spawner(mGame);

  mGame->UpdateCamera();
}

void Level1::Update(float deltaTime) {
  UIScreen::Update(deltaTime);

  const Player* player = GetGame()->GetPlayer();
  uint32_t curretHP = player->GetCurrentHP();

  if (curretHP <= 0) {
    mGame->SetScene(GameScene::GameOver);
  }
}
