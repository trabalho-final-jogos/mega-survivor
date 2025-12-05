// BoomerangProjectile.h
#pragma once
#include "../Projectile.h"  // A classe base que herda de Actor

class BoomerangProjectile : public Projectile {
 public:
  BoomerangProjectile(class Game* game, int width, int height);

  void Awake(Actor* owner,
             const Vector2& position,
             float rotation,
             float lifetime,
             const Vector2& velocity,
             float damage,
             float areaScale) override;

  void OnUpdate(float deltaTime) override;

  void OnHorizontalCollision(const float minOverlap,
                             AABBColliderComponent* other) override;
  void OnVerticalCollision(const float minOverlap,
                           AABBColliderComponent* other) override;

 private:
  // Enum para controlar o estado do bumerangue
  enum class BoomerangState {
    Extending,  // Indo (se afastando)
    Returning   // Voltando
  };

  BoomerangState mState;
  float mDistanceTraveled;  // Distância percorrida na fase "Extending"

  Vector2 mStartPosition;  // p0 (Ponto inicial)
  Vector2 mControlPoint;
  float mTimeElapsed;
  const float mMaxRange = 250.0f;  // Distância do ponto de controle
  const float mSpinSpeed;


};