#pragma once
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Components/Physics/AABBColliderComponent.h"
#include "../Components/Physics/RigidBodyComponent.h"
#include "Actor.h"
#include "Player.h"
#include "weapons/Projectile.h"

constexpr float ATTACK_DELAY = 1.5f;

class Enemy : public Actor {
 public:
  Enemy(class Game* game, int health = 1, uint16_t xpDrop = 10);

  int GetHealth() const { return mHealth; }
  uint16_t GetXPDrop() const { return mXPDrop; }
  float GetDamage() const { return mDamage; }
  void SetXPDrop(uint16_t xpDrop) { mXPDrop = xpDrop; }

  void OnHorizontalCollision(const float minOverlap,
                             class AABBColliderComponent* other) override;
  void OnVerticalCollision(const float minOverlap,
                           class AABBColliderComponent* other) override;

  virtual void TakeDamage(float damage);

 protected:
  int mHealth{1};
  uint16_t mXPDrop;
  float mDamage{1};
  float mLastHitTime{0.0f};
  class RigidBodyComponent* mRigidBody;
  class AnimatorComponent* mDrawComponent;
  class AABBColliderComponent* mColliderComponent;

 private:
};