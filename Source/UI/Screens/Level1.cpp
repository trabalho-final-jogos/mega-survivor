#include "Level1.h"
#include "../../Game.h"
#include "../../Actors/Player.h"

Level1::Level1(Game* game, const std::string& fontName) : UIScreen(game, fontName) {
  // 1. Load Level Data
  int** level = mGame->LoadLevel("../Assets/Levels/Level1-1/level1-1.csv", Game::LEVEL_WIDTH, Game::LEVEL_HEIGHT);
  mGame->SetLevelData(level);

  // 2. Build Level (create blocks, enemies, etc.)
  mGame->BuildLevel(level, Game::LEVEL_WIDTH, Game::LEVEL_HEIGHT);

  // 3. Create Player (Mario)
  // Check if player already exists (shouldn't if cleaned up correctly)
  if (!mGame->GetPlayer()) {
      Player* mario = new Player(mGame);
      mario->SetPosition(Vector2(Game::TILE_SIZE / 2, Game::TILE_SIZE / 2));
      mGame->SetPlayer(mario);
  }

  // 4. Update Camera to center on Mario immediately
  mGame->UpdateCamera();

  // Note: Music is currently played in Game::SetScene, so we don't need to play it here.
}
