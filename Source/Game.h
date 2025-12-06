// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL.h>
#include <vector>
#include "Actors/Player.h"
#include "Audio/AudioSystem.h"
#include "Components/Upgrades/UpgradeComponent.h"
#include "Renderer/Renderer.h"

constexpr static std::string_view GAME_FONT{"../Assets/Fonts/MMRock9.ttf"};

enum class GameScene { MainMenu, UpgradeStore, Level1, CharSelect, GameOver };

class Game {
 public:
  Game();

  bool Initialize();
  void RunLoop();
  void Shutdown();
  void Quit() { mIsRunning = false; }

  // Actor functions
  void InitializeActors();
  void UpdateActors(float deltaTime);
  void AddActor(class Actor* actor);
  void RemoveActor(class Actor* actor);

  void SetScene(GameScene scene);
  void UnloadScene();
  void ResetGame();

  // Renderer
  class Renderer* GetRenderer() { return mRenderer; }

  // Character
  PlayerChar mChar = MEGAMAN;

  static const int WINDOW_WIDTH = 1280;
  static const int WINDOW_HEIGHT = 720;
  static const int VIRTUAL_WIDTH = 900;
  static const int VIRTUAL_HEIGHT = 506;
  static const int LEVEL_WIDTH = 60;
  static const int LEVEL_HEIGHT = 60;
  static const int TILE_SIZE = 32;
  static const int SPAWN_DISTANCE = 700;
  static const int FPS = 60;
  static constexpr float SELECTED_OPACITY = 0.9;
  static constexpr float UNSELECTED_OPACITY = 0.5;

  // Draw functions
  void AddDrawable(class DrawComponent* drawable);
  void RemoveDrawable(class DrawComponent* drawable);
  std::vector<class DrawComponent*>& GetDrawables() { return mDrawables; }

  // UI functions
  void PushUI(class UIScreen* screen) { mUIStack.emplace_back(screen); }
  const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }

  void SetPaused(bool paused) { mIsPaused = paused; }
  bool IsPaused() const { return mIsPaused; }

  // Collider functions
  void AddCollider(class AABBColliderComponent* collider);
  void RemoveCollider(class AABBColliderComponent* collider);
  std::vector<class AABBColliderComponent*>& GetColliders() {
    return mColliders;
  }

  // Camera functions
  Vector2& GetCameraPos() { return mCameraPos; };
  void SetCameraPos(const Vector2& position) { mCameraPos = position; };

  // Game specific
  class Player* GetPlayer() { return mPlayer; }
  void SetPlayer(Player* player) { mPlayer = player; }

  const Vector2& GetMousePos() const { return mMouseWorldPos; }  // Getter

  // Level loading
  int** LoadLevel(const std::string& fileName, int width, int height);
  void BuildLevel(int** levelData, int width, int height);
  void SetLevelData(int** data) { mLevelData = data; }

  void UpdateCamera();
  void UpdateRunTime();
  void StartClock();
  void StopClock();
  void ResetClock();
  float GetClockTime();

  uint8_t GetRunSeconds() const { return mRunSeconds; }
  uint8_t GetRunMinutes() const { return mRunMinutes; }
  void SetPlayerCharInfo(CharInfo pCharInfo) { mCharInfo = pCharInfo; }
  CharInfo GetCharInfo() const { return mCharInfo; }
  // Persistent Upgrades
  // Using a raw pointer here, but since Game isn't an Actor, we can't use
  // Component's lifecycle fully. However, we can just use the class as data
  // container. But UpgradeComponent ctor requires an Actor. I will make a
  // getter that returns the raw data component. Wait, if I cannot instantiate
  // UpgradeComponent without an Actor, I have a problem. I will modify
  // UpgradeComponent to allow nullptr owner, or create a dummy actor. Or I can
  // add a specialized "UpgradeData" struct. For now, I'll assume I can hack it
  // or I'll fix UpgradeComponent ctor.

  // Actually, I should probably manage currency here too.
  int GetCurrency() const { return mCurrency; }
  void AddCurrency(int amount) { mCurrency += amount; }

  // This will hold the persistent stats
  class UpgradeComponent* GetPersistentUpgrades() {
    return mPersistentUpgrades;
  }

 private:
  void ProcessInput();
  void UpdateGame(float deltaTime);
  void GenerateOutput();
  void UpdateMouseWorldPos();
  void PerformSceneChange();

  // All the actors in the game
  std::vector<class Actor*> mActors;
  std::vector<class Actor*> mPendingActors;

  // Camera
  Vector2 mCameraPos;

  // All the draw components
  std::vector<class DrawComponent*> mDrawables;

  // All the collision components
  std::vector<class AABBColliderComponent*> mColliders;

  // SDL stuff
  SDL_Window* mWindow;
  class Renderer* mRenderer;

  // All UI screens in the game
  std::vector<class UIScreen*> mUIStack;

  // Audio system
  AudioSystem* mAudio;

  // Track elapsed time since game start
  Uint32 mTicksCount;

  GameScene mCurrentScene;

  // Track if we're updating actors right now
  bool mIsRunning;
  bool mIsDebugging;
  bool mUpdatingActors;
  bool mIsPaused{false};
  bool mPendingSceneChange{false};

  // Game-specific
  class Player* mPlayer;
  int** mLevelData;

  Vector2 mMouseWorldPos;

  GameScene mNextScene;

  uint16_t mRunTotalSeconds{0};
  uint8_t mRunSeconds{0};
  uint8_t mRunMinutes{0};

  Uint32 mClockStartTime;
  bool mIsClockRunning;

  CharInfo mCharInfo;
  int mCurrency{100000};  // Start with some money for testing
  class UpgradeComponent* mPersistentUpgrades = nullptr;
  class Actor* mPersistentActor = nullptr;  // Dummy actor to hold the component
};
