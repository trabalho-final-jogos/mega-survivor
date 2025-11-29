#pragma once
#include "../Enemy.h"

class Bat : public Enemy {
 public:
  Bat(class Game* game);
  void OnUpdate(float deltaTime) override;
  void OnHorizontalCollision(const float minOverlap,
                             class AABBColliderComponent* other) override;
  void OnVerticalCollision(const float minOverlap,
                           class AABBColliderComponent* other) override;

  // Define a velocidade fixa (chamado pelo Spawner)
  void SetFixedVelocity(const Vector2& velocity);

  void SetStats(float health);
};