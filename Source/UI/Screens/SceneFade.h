// SceneFade.h
#pragma once

#include "../../Game.h"
#include "UIScreen.h"

class SceneFade : public UIScreen {
 public:
  // nextScene: cena que será carregada no meio do fade
  // duration: tempo total do cross-fade em segundos
  SceneFade(Game* game, GameScene nextScene, float duration = 1.0f);

  void Update(float deltaTime) override;

 private:
  GameScene mNextScene;
  float mDuration;  // total do efeito
  float mElapsed;   // tempo acumulado
  bool mSceneSwitched;

  class UIRect* mFadeRect;
};
