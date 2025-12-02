// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include "Actors/Actor.h"
#include "Actors/Block.h"
#include "Actors/Player.h"
#include "Actors/Spawner.h"
#include "CSV.h"
#include "Components/Drawing/DrawComponent.h"
#include "Components/Physics/RigidBodyComponent.h"
#include "Managers/ColorPalette.h"
#include "Random.h"
#include "UI/Screens/CharSelection.h"
#include "UI/Screens/GameOver.h"
#include "UI/Screens/HUD.h"
#include "UI/Screens/Level1.h"
#include "UI/Screens/MainMenu.h"
#include "UI/Screens/PausedMenu.h"
#include "UI/Screens/UpgradeStore.h"

Game::Game()
    : mWindow(nullptr),
      mRenderer(nullptr),
      mTicksCount(0),
      mIsRunning(true),
      mAudio(nullptr),
      mIsDebugging(false),
      mUpdatingActors(false),
      mCameraPos(Vector2::Zero),
      mPlayer(nullptr),
      mLevelData(nullptr),
      mMouseWorldPos(Vector2::Zero),
      mClockStartTime(0),
      mIsClockRunning(false),
      mPendingSceneChange(false) {}

bool Game::Initialize() {
  Random::Init();

  ColorPalette::GetInstance().InitializeBasePalette();

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  // Init SDL_Image
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    SDL_Log("Unable to initialize SDL_image: %s", IMG_GetError());
    return false;
  }

  // Initialize SDL_ttf
  if (TTF_Init() != 0) {
    SDL_Log("Failed to initialize SDL_ttf");
    return false;
  }

  mWindow = SDL_CreateWindow("Mega Survivors", 0, 0, WINDOW_WIDTH,
                             WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
  if (!mWindow) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  mRenderer = new Renderer(mWindow);
  mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

  // Init all game actors
  InitializeActors();

  mTicksCount = SDL_GetTicks();
  mIsDebugging = true;

  mAudio = new AudioSystem(16);

  SetScene(GameScene::MainMenu);

  return true;
}

void Game::InitializeActors() {
  // mRenderer->DrawDebugRect();
}

int** Game::LoadLevel(const std::string& fileName, int width, int height) {
  int** levelData = new int*[height];
  for (int i = 0; i < height; ++i) {
    levelData[i] = new int[width];

    for (int j = 0; j < width; ++j) {
      levelData[i][j] = -1;
    }
  }

  // 2. Abre o arquivo para leitura
  std::ifstream file(fileName);
  if (!file.is_open()) {
    SDL_Log("Erro: Nao foi possivel abrir o arquivo de nivel '%s'",
            fileName.c_str());
    for (int i = 0; i < height; ++i) {
      delete[] levelData[i];
    }
    delete[] levelData;
    return nullptr;
  }

  std::string line;
  int currentRow = 0;
  while (std::getline(file, line)) {
    if (currentRow >= height) {
      break;
    }

    std::vector<int> parsedRow = CSVHelper::Split(line);

    if (parsedRow.size() != width) {
      SDL_Log(
          "Erro na linha %d do nivel: largura esperada %d, mas encontrou %zu",
          currentRow, width, parsedRow.size());
      continue;
    }

    for (int col = 0; col < width; ++col) {
      levelData[currentRow][col] = parsedRow[col];
    }

    currentRow++;
  }

  file.close();

  SDL_Log("Nivel '%s' carregado com sucesso.", fileName.c_str());
  return levelData;
}

void Game::BuildLevel(int** levelData, int width, int height) {
  if (!levelData) {
    SDL_Log(
        "ERRO: Tentativa de construir nível sem dados carregados (mLevelData é "
        "nulo).");
    return;
  }

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int tileID = levelData[i][j];

      if (tileID < 0) {
        continue;
      }

      Vector2 pos(static_cast<float>(j * TILE_SIZE) + (TILE_SIZE / 2.0f),
                  static_cast<float>(i * TILE_SIZE) + (TILE_SIZE / 2.0f));

      switch (tileID) {
        // --- PERSONAGEM ---
        case 16:
          SDL_Log("Player created at position (%.1f, %.1f)", pos.x, pos.y);
          mPlayer = new Player(this, mChar);
          mPlayer->SetPosition(pos);
          break;

        case 11: {
          auto b = new Block(this, "../Assets/Sprites/Chao/071.png");
          b->SetPosition(pos);
        } break;
        case 9: {
          auto b = new Block(this, "../Assets/Sprites/Chao/0052.png");
          b->SetPosition(pos);
        } break;
        case 10: {
          auto b = new Block(this, "../Assets/Sprites/Chao/0053.png");
          b->SetPosition(pos);
        } break;
        case 6: {
          auto b = new Block(this, "../Assets/Sprites/Chao/005.png");
          b->SetPosition(pos);
        } break;
        case 8: {
          auto b = new Block(this, "../Assets/Sprites/Chao/0051.png");
          b->SetPosition(pos);
        } break;
        case 5: {
          auto b = new Block(this, "../Assets/Sprites/Chao/0023.png");
          b->SetPosition(pos);
        } break;
        case 1: {
          auto b = new Block(this, "../Assets/Sprites/Chao/0021.png");
          b->SetPosition(pos);
        } break;
        case 2: {
          auto b = new Block(this, "../Assets/Sprites/Chao/0024.png");
          b->SetPosition(pos);
        } break;
        case 4: {
          auto b = new Block(this, "../Assets/Sprites/Chao/0022.png");
          b->SetPosition(pos);
        } break;
      }
    }
  }
}

void Game::RunLoop() {
  while (mIsRunning) {
    // Calculate delta time in seconds
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
      deltaTime = 0.05f;
    }

    mTicksCount = SDL_GetTicks();

    ProcessInput();
    UpdateGame(deltaTime);
    GenerateOutput();

    // Sleep to maintain frame rate
    int sleepTime = (1000 / FPS) - (SDL_GetTicks() - mTicksCount);
    if (sleepTime > 0) {
      SDL_Delay(sleepTime);
    }
  }
}

void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        Quit();
        break;

      case SDL_KEYDOWN:
        if (event.key.repeat == 0) {
          if (!mUIStack.empty()) {
            mUIStack.back()->HandleKeyPress(event.key.keysym.sym);
          }

          break;
        }
    }

    if (!mIsPaused) {
      // Handle actors (continuous keys)
      const Uint8* state = SDL_GetKeyboardState(nullptr);
      for (auto actor : mActors) {
        actor->ProcessInput(state);
      }
    }
  }
}

void Game::UpdateGame(float deltaTime) {
  if (mPendingSceneChange) {
    PerformSceneChange();
    mPendingSceneChange = false;
  }

  for (auto* ui : mUIStack) {
    if (ui->GetState() == UIScreen::UIState::Active) {
      ui->Update(deltaTime);
    }
  }

  // Update all actors and pending actors
  if (!mIsPaused) {
    UpdateActors(deltaTime);

    // Update run time
    UpdateRunTime();

    // Update camera position
    UpdateCamera();
  }

  for (auto* ui : mUIStack) {
    if (ui->GetState() == UIScreen::UIState::Active) {
      ui->Update(deltaTime);
    }
  }

  auto iter = mUIStack.begin();
  while (iter != mUIStack.end()) {
    if ((*iter)->GetState() == UIScreen::UIState::Closing) {
      delete *iter;
      iter = mUIStack.erase(iter);
    } else {
      iter++;
    }
  }

  UpdateMouseWorldPos();
}

void Game::UpdateRunTime() {
  if (mIsClockRunning) {
    float totalTime = GetClockTime();

    int totalSeconds = static_cast<int>(totalTime);

    static int lastSecond = -1;

    if (totalSeconds != lastSecond) {
      SDL_Log("Tempo: %02d:%02d", mRunMinutes, mRunSeconds);
      lastSecond = totalSeconds;

      mRunSeconds = mRunTotalSeconds % 60;

      mRunMinutes = mRunTotalSeconds / 60;

      mRunTotalSeconds++;
    }
  }
}

void Game::UpdateActors(float deltaTime) {
  mUpdatingActors = true;
  for (auto actor : mActors) {
    actor->Update(deltaTime);
  }
  mUpdatingActors = false;

  for (auto pending : mPendingActors) {
    mActors.emplace_back(pending);
  }
  mPendingActors.clear();

  std::vector<Actor*> deadActors;
  for (auto actor : mActors) {
    if (actor->GetState() == ActorState::Destroy) {
      deadActors.emplace_back(actor);
    }
  }

  for (auto actor : deadActors) {
    delete actor;
  }
}

void Game::ResetGame() {
  UnloadScene();
  StopClock();
  ResetClock();
  SetScene(GameScene::MainMenu);
}

void Game::UpdateCamera() {
  if (!mPlayer) {
    return;
  }

  Vector2 marioPos = mPlayer->GetPosition();

  // --- USA A RESOLUÇÃO VIRTUAL ---
  float halfScreenWidth = VIRTUAL_WIDTH / 2.0f;
  float halfScreenHeight = VIRTUAL_HEIGHT / 2.0f;
  // --------------------------------

  // 1. Centraliza a câmera no Mario
  mCameraPos.x = marioPos.x - halfScreenWidth;
  mCameraPos.y = marioPos.y - halfScreenHeight;

  // 2. Clamping (Limites)
  float levelWidth = LEVEL_WIDTH * TILE_SIZE;
  float levelHeight = LEVEL_HEIGHT * TILE_SIZE;

  mCameraPos.x = std::max(0.0f, mCameraPos.x);  // Trava Esquerda

  // --- USA A RESOLUÇÃO VIRTUAL ---
  mCameraPos.x =
      std::min(mCameraPos.x, levelWidth - VIRTUAL_WIDTH);  // Trava Direita

  mCameraPos.y = std::max(0.0f, mCameraPos.y);  // Trava Topo

  // --- USA A RESOLUÇÃO VIRTUAL ---
  mCameraPos.y =
      std::min(mCameraPos.y, levelHeight - VIRTUAL_HEIGHT);  // Trava Fundo
}

void Game::AddActor(Actor* actor) {
  if (mUpdatingActors) {
    mPendingActors.emplace_back(actor);
  } else {
    mActors.emplace_back(actor);
  }
}

void Game::RemoveActor(Actor* actor) {
  auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
  if (iter != mPendingActors.end()) {
    // Swap to end of vector and pop off (avoid erase copies)
    std::iter_swap(iter, mPendingActors.end() - 1);
    mPendingActors.pop_back();
  }

  iter = std::find(mActors.begin(), mActors.end(), actor);
  if (iter != mActors.end()) {
    // Swap to end of vector and pop off (avoid erase copies)
    std::iter_swap(iter, mActors.end() - 1);
    mActors.pop_back();
  }
}

void Game::AddDrawable(class DrawComponent* drawable) {
  mDrawables.emplace_back(drawable);

  std::sort(mDrawables.begin(), mDrawables.end(),
            [](DrawComponent* a, DrawComponent* b) {
              return a->GetDrawOrder() < b->GetDrawOrder();
            });
}

void Game::RemoveDrawable(class DrawComponent* drawable) {
  auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
  mDrawables.erase(iter);
}

void Game::AddCollider(class AABBColliderComponent* collider) {
  mColliders.emplace_back(collider);
}

void Game::RemoveCollider(AABBColliderComponent* collider) {
  auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
  mColliders.erase(iter);
}

void Game::GenerateOutput() {
  // Clear back buffer
  mRenderer->Clear();

  for (auto drawable : mDrawables) {
    drawable->Draw(mRenderer);
  }

  // Draw UI screens
  mRenderer->DrawUI();

  // Swap front buffer and back buffer
  mRenderer->Present();
}

void Game::UpdateMouseWorldPos() {
  int screenX, screenY;
  SDL_GetMouseState(&screenX, &screenY);

  // 2. Converte para a Resolução VIRTUAL (ex: 0-640)
  float virtualX =
      static_cast<float>(screenX) *
      (static_cast<float>(VIRTUAL_WIDTH) / static_cast<float>(WINDOW_WIDTH));
  float virtualY =
      static_cast<float>(screenY) *
      (static_cast<float>(VIRTUAL_HEIGHT) / static_cast<float>(WINDOW_HEIGHT));

  // 3. Converte para Coordenadas do MUNDO (adicionando a câmera)
  mMouseWorldPos.x = virtualX + mCameraPos.x;
  mMouseWorldPos.y = virtualY + mCameraPos.y;
}

void Game::SetScene(GameScene nextScene) {
  mNextScene = nextScene;
  mPendingSceneChange = true;
}

void Game::PerformSceneChange() {
  if (mAudio) {
    mAudio->StopAllSounds();
  }

  UnloadScene();

  mCurrentScene = mNextScene;

  switch (mNextScene) {
    case GameScene::MainMenu: {
      new MainMenu(this, std::string(GAME_FONT));
      break;
    }

    case GameScene::Level1: {
      new Level1(this, std::string(GAME_FONT));

      if (mAudio) {
        mAudio->PlaySound("Music.ogg", true);
      }

      break;
    }

    case GameScene::UpgradeStore: {
      new UpgradeStore(this, std::string(GAME_FONT));
      break;
    }

    case GameScene::CharSelect: {
      new CharSelection(this, std::string(GAME_FONT));
      break;
    }

    case GameScene::GameOver: {
      new GameOver(this, std::string(GAME_FONT));
      break;
    }

    default:
      break;
  }
}

void Game::UnloadScene() {
  // Use state so we can call this from withing an a actor update
  while (!mActors.empty()) {
    Actor* actor = mActors.back();
    mActors.pop_back();
    delete actor;
  }

  StopClock();
  ResetClock();

  mIsPaused = false;

  mActors.clear();
  mPendingActors.clear();
  mPlayer = nullptr;

  // Delete UI screens
  for (auto ui : mUIStack) {
    delete ui;
  }
  mUIStack.clear();
}

void Game::Shutdown() {
  float i = 0.f;
  while (!mActors.empty()) {
    Actor* actor = mActors.back();
    mActors.pop_back();  // remove do vetor ANTES de deletar
    delete actor;        // crash provavelmente aqui
  }
  // Delete level data
  if (mLevelData) {
    for (int i = 0; i < LEVEL_HEIGHT; ++i) {
      delete[] mLevelData[i];
    }
    delete[] mLevelData;
    mLevelData = nullptr;
  }
  mRenderer->Shutdown();
  delete mRenderer;
  mRenderer = nullptr;

  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void Game::StartClock() {
  mIsClockRunning = true;
  mClockStartTime = SDL_GetTicks();

  SDL_Log("Relógio iniciado!");
}

void Game::StopClock() {
  mIsClockRunning = false;
}

void Game::ResetClock() {
  mRunTotalSeconds = 0;
  mRunSeconds = 0;
  mRunMinutes = 0;
}

float Game::GetClockTime() {
  if (!mIsClockRunning) {
    return 0.0f;
  }
  Uint32 currentTime = SDL_GetTicks();
  Uint32 elapsedTimeMS = currentTime - mClockStartTime;

  return static_cast<float>(elapsedTimeMS) / 1000.0f;
}
