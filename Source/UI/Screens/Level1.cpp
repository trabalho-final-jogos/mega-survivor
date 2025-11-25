#include "Level1.h"
#include "../../Actors/Player.h"
#include "../../Game.h"

Level1::Level1(Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  int** level = mGame->LoadLevel("../Assets/Levels/Level1-1/level1-1.csv",
                                 Game::LEVEL_WIDTH, Game::LEVEL_HEIGHT);
  mGame->SetLevelData(level);

  SDL_Log("Level data loaded for Level 1-1");

  mGame->BuildLevel(level, Game::LEVEL_WIDTH, Game::LEVEL_HEIGHT);

  SDL_Log("Level 1-1 built successfully");

  if (!mGame->GetPlayer()) {
    Player* player = new Player(mGame);
    player->SetPosition(Vector2(Game::TILE_SIZE / 2, Game::TILE_SIZE / 2));
    mGame->SetPlayer(player);
  }

  mGame->UpdateCamera();
}
