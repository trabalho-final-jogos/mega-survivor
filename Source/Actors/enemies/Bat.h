#pragma once
#include "../Enemy.h"

class Bat : public Enemy {
 public:
  Bat(class Game* game);
  void OnUpdate(float deltaTime) override;

  // Define a velocidade fixa (chamado pelo Spawner)
  void SetFixedVelocity(const Vector2& velocity);

  void SetStats(float health);
};