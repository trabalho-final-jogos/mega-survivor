#pragma once
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Components/Physics/AABBColliderComponent.h"
#include "../Components/Physics/RigidBodyComponent.h"
#include "../Game.h"
#include "Actor.h"

class XPGem : public Actor {
 public:
  XPGem(Game* game, const Vector2& position, int xpValue);

  void OnUpdate(float deltaTime) override;
  void OnHorizontalCollision(float minOverlap,
                             AABBColliderComponent* other) override;
  void OnVerticalCollision(float minOverlap,
                           AABBColliderComponent* other) override;

  uint32_t GetXPValue() const { return mXPValue; }

 private:
  uint32_t mXPValue;
  float mMagnetRadius;
  float mMoveSpeed;

  RigidBodyComponent* mRigidBody;
  AABBColliderComponent* mCollider;
  AnimatorComponent* mAnimator;
};
