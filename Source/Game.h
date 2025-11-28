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
#include "Audio/AudioSystem.h"
#include "Renderer/Renderer.h"

constexpr static std::string_view GAME_FONT{"../Assets/Fonts/MMRock9.ttf"};

enum class GameScene { MainMenu, PausedMenu, UpgradeStore, Level1 };

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

  // Renderer
  class Renderer* GetRenderer() { return mRenderer; }

  static const int WINDOW_WIDTH = 1280;
  static const int WINDOW_HEIGHT = 720;
  static const int VIRTUAL_WIDTH = 640;
  static const int VIRTUAL_HEIGHT = 360;
  static const int LEVEL_WIDTH = 60;
  static const int LEVEL_HEIGHT = 60;
  static const int TILE_SIZE = 32;
  static const int SPAWN_DISTANCE = 700;
  static const int FPS = 60;

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
  const class Player* GetPlayer() { return mPlayer; }
  void SetPlayer(class Player* player) { mPlayer = player; }

  const Vector2& GetMousePos() const { return mMouseWorldPos; }  // Getter

  // Level loading
  int** LoadLevel(const std::string& fileName, int width, int height);
  void BuildLevel(int** levelData, int width, int height);
  void SetLevelData(int** data) { mLevelData = data; }

  void UpdateCamera();
	void StartClock();
	float GetClockTime();

 private:
  void ProcessInput();
  void UpdateGame(float deltaTime);
  void GenerateOutput();
  void UpdateMouseWorldPos();

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

  // Track if we're updating actors right now
  bool mIsRunning;
  bool mIsDebugging;
  bool mUpdatingActors;
  bool mIsPaused{false};

  // Game-specific
  class Player* mPlayer;
  int** mLevelData;

  Vector2 mMouseWorldPos;

	Uint32 mClockStartTime;
	bool mIsClockRunning;
};
