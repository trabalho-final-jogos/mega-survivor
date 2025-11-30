#pragma once
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Components/Physics/AABBColliderComponent.h"
#include "../Components/Physics/RigidBodyComponent.h"
#include "Actor.h"
#include "weapons/Projectile.h"

class Enemy : public Actor {
 public:
  Enemy(class Game* game, int health = 1, uint16_t xpDrop = 10);

  int GetHealth() const { return mHealth; }
  uint16_t GetXPDrop() const { return mXPDrop; }
  void SetXPDrop(uint16_t xpDrop) { mXPDrop = xpDrop; }

  void OnHorizontalCollision(const float minOverlap,
                             class AABBColliderComponent* other) override;
  void OnVerticalCollision(const float minOverlap,
                           class AABBColliderComponent* other) override;

  virtual void TakeDamage(int damage);

 protected:
  int mHealth{1};
  uint16_t mXPDrop;
  uint32_t mDamage{1};
  class RigidBodyComponent* mRigidBody;
  class AnimatorComponent* mDrawComponent;
  class AABBColliderComponent* mColliderComponent;

 private:
};