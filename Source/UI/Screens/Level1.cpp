#include "Level1.h"
#include "../../Actors/Player.h"
#include "../../Actors/Spawner.h"
#include "../../Game.h"

Level1::Level1(Game* game, const std::string& fontName)
    : UIScreen(game, fontName) {
  int** level = mGame->LoadLevel("../Assets/Levels/Level1/chao.csv", 60, 60);
  mGame->SetLevelData(level);

  SDL_Log("Level data loaded for Level 1-1");

  mGame->BuildLevel(level, 60, 60);

  SDL_Log("Level 1-1 built successfully");

    if (!mGame->GetPlayer()) {
        Player* player = new Player(mGame);
        player->SetPosition(Vector2(Game::WINDOW_WIDTH / 2, Game::WINDOW_HEIGHT / 2));
        mGame->SetPlayer(player);
    }
		mGame->StartClock();
    new Spawner(mGame);

  mGame->UpdateCamera();
}
