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
#include "Actors/Goomba.h"
#include "Actors/Player.h"
#include "Actors/QuestionBlock.h"
#include "Actors/Spawner.h"
#include "CSV.h"
#include "Components/Drawing/DrawComponent.h"
#include "Components/Physics/RigidBodyComponent.h"
#include "Random.h"
#include "UI/Screens/HUD.h"
#include "UI/Screens/MainMenu.h"
#include "UI/Screens/PausedMenu.h"

Game::Game()
    : mWindow(nullptr),
      mRenderer(nullptr),
      mTicksCount(0),
      mIsRunning(true),
      mIsDebugging(false),
      mUpdatingActors(false),
      mCameraPos(Vector2::Zero),
      mMario(nullptr),
      mLevelData(nullptr),
      mMouseWorldPos(Vector2::Zero) {}

bool Game::Initialize() {
  Random::Init();

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow("TP3: Super Mario Bros", 0, 0, WINDOW_WIDTH,
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
  return true;
}

void Game::InitializeActors() {
  // mRenderer->DrawDebugRect();
  // mLevelData = LoadLevel("../Assets/Levels/Level1-1/level1-1.csv",
  // LEVEL_WIDTH, LEVEL_HEIGHT); BuildLevel(mLevelData,
  // LEVEL_WIDTH,LEVEL_HEIGHT);
  SDL_Log("criou o mario");
  mMario = new Player(this);
  mMario->SetPosition(Vector2(TILE_SIZE / 2, TILE_SIZE / 2));
  UpdateCamera();
  if (mLevelData) {
    SDL_Log("--- Imprimindo Matriz Completa do Nível (%d x %d) ---",
            LEVEL_HEIGHT, LEVEL_WIDTH);

    for (int i = 0; i < LEVEL_HEIGHT; ++i) {
      std::stringstream ss;

      for (int j = 0; j < LEVEL_WIDTH; ++j) {
        ss << mLevelData[i][j] << ",";
      }

      SDL_Log("%s", ss.str().c_str());
    }

    SDL_Log("--- Fim da Matriz do Nível ---");
  } else {
    SDL_Log(
        "ERRO: mLevelData é nulo. A matriz do nível não foi carregada e não "
        "pode ser impressa.");
  }
}

int** Game::LoadLevel(const std::string& fileName, int width, int height) {
  int** levelData = new int*[height];
  for (int i = 0; i < height; ++i) {
    levelData[i] = new int[width];
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
          SDL_Log("criou o mario");
          mMario = new Player(this);
          mMario->SetPosition(pos);
          break;

        case 10: {
          auto spawner = new Spawner(this, 400.f);
          spawner->SetPosition(pos);
        } break;
        case 0: {
          auto b = new Block(this, "../Assets/Sprites/Blocks/BlockA.png");
          b->SetPosition(pos);
        } break;
        case 1: {
          auto b = new QuestionBlock(
              this, "../Assets/Sprites/Blocks/BlockC.png", "Block Coin");
          b->SetPosition(pos);
        } break;
        case 2: {
          auto b = new Block(this, "../Assets/Sprites/Blocks/BlockF.png");
          b->SetPosition(pos);
        } break;
        case 3: {
          auto b = new Block(this, "../Assets/Sprites/Collectables/Coin.png");
          b->SetPosition(pos);
        } break;
        case 4: {
          auto b =
              new Block(this, "../Assets/Sprites/Blocks/BlockB.png", "Block B");
          b->SetPosition(pos);
        } break;
        case 5: {
          auto b = new Block(this, "../Assets/Sprites/Blocks/BlockE.png");
          b->SetPosition(pos);
        } break;
        case 6: {
          auto b = new Block(this, "../Assets/Sprites/Blocks/BlockI.png");
          b->SetPosition(pos);
        } break;
        // case 7: { auto b = new Block(this,
        // "../Assets/Sprites/Blocks/BlockH.png"); b->SetPosition(pos); } break;
        case 8: {
          auto b = new Block(this, "../Assets/Sprites/Blocks/BlockD.png");
          b->SetPosition(pos);
        } break;
        case 9: {
          auto b = new Block(this, "../Assets/Sprites/Blocks/BlockH.png");
          b->SetPosition(pos);
        } break;

        // case 11: { auto b = new Block(this,
        // "../Assets/Sprites/Blocks/BlockL.png"); b->SetPosition(pos); } break;
        case 12: {
          auto b = new Block(this, "../Assets/Sprites/Blocks/BlockG.png");
          b->SetPosition(pos);
        } break;
        case 13: {
          auto b = new QuestionBlock(
              this, "../Assets/Sprites/Blocks/BlockC.png", "Block Mushroom");
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
    }
  }

  const Uint8* state = SDL_GetKeyboardState(nullptr);

  for (auto actor : mActors) {
    actor->ProcessInput(state);
  }
}

void Game::UpdateGame(float deltaTime) {
  // Update all actors and pending actors
  UpdateActors(deltaTime);

  // Update camera position
  UpdateCamera();

  UpdateMouseWorldPos();
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

void Game::UpdateCamera() {
  if (!mMario) {
    return;
  }

  Vector2 marioPos = mMario->GetPosition();

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

  Texture* bgTexture =
      mRenderer->GetTexture("../Assets/Sprites/Background.png");

  if (bgTexture) {
    float bgWidth = static_cast<float>(bgTexture->GetWidth());
    float bgHeight = static_cast<float>(bgTexture->GetHeight());

    Vector2 bgPos(bgWidth / 2.0f, bgHeight / 2.0f);

    Vector2 bgSize(bgWidth, bgHeight);

    mRenderer->DrawTexture(
        bgPos,                            // Posição do CENTRO do fundo no MUNDO
        bgSize,                           // Tamanho TOTAL do fundo
        0.0f,                             // Rotação
        Vector3(1.0f, 1.0f, 1.0f),        // Cor (branco)
        bgTexture,                        // A textura grande
        Vector4(0.0f, 0.0f, 1.0f, 1.0f),  // textureRect (usa a textura inteira)
        GetCameraPos(),                   // <<--- PASSA A CÂMERA ATUAL
        false,                            // flip
        1.0f                              // textureFactor
    );
  } else {
    SDL_Log("Erro: Nao foi possivel carregar a textura do fundo.");
  }

  for (auto drawable : mDrawables) {
    drawable->Draw(mRenderer);

    if (mIsDebugging) {
      // Call draw for actor components
      for (auto comp : drawable->GetOwner()->GetComponents()) {
        comp->DebugDraw(mRenderer);
      }
    }
  }

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
  if (mAudio) {
    mAudio->StopAllSounds();
  }

  UnloadScene();

  switch (nextScene) {
    case GameScene::MainMenu: {
      new MainMenu(this, GAME_FONT.data());
      break;
    }
    case GameScene::Level1: {
      /*
      mCamera = new Camera(this, Vector3(-300.0f, 0.0f, 0.0f),
                           Vector3(20.0f, 0.0f, 0.0f),
                           Vector3(0.0f, 0.0f, 1.0f), 70.0f, 10.0f, 10000.0f);

      mHUD = new HUD(this, GAME_FONT.data());
      if (mHUD) {
        mHUD->SetHealth(mShip->GetHealth());
        mHUD->SetScore(mScore);
      }
      */

      if (mAudio) {
        mAudio->PlaySound("Music.ogg", true);
      }

      break;
    }
    default:
      break;
  }
}

void Game::UnloadScene() {
  // Use state so we can call this from withing an a actor update
  for (auto* actor : mActors) {
    actor->SetState(ActorState::Destroy);
  }

  // Delete UI screens
  for (auto ui : mUIStack) {
    delete ui;
  }
  mUIStack.clear();
}

void Game::Shutdown() {
  float i = 0.f;
  SDL_Log("Game::Shutdown start");
  while (!mActors.empty()) {
    Actor* actor = mActors.back();
    SDL_Log("Deleting actor: %s (%p)", typeid(*actor).name(), (void*)actor);
    mActors.pop_back();  // remove do vetor ANTES de deletar
    delete actor;        // crash provavelmente aqui
    SDL_Log("Deleted actor: %p", (void*)actor);
  }
  SDL_Log("ATORES");
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
