//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "Actor.h"
#include "Aim.h"
#include "weapons/WeaponType.h"

constexpr float PLAYER_BASE_SPEED{200.0f};

class Player : public Actor {
 public:
  explicit Player(Game* game,
                  float forwardSpeed = PLAYER_BASE_SPEED,
                  float jumpSpeed = -750.0f);

  void OnProcessInput(const Uint8* keyState) override;
  void OnUpdate(float deltaTime) override;

  void OnHorizontalCollision(const float minOverlap,
                             AABBColliderComponent* other) override;
  void OnVerticalCollision(const float minOverlap,
                           AABBColliderComponent* other) override;

  void EquipWeapon(WeaponType type);

  void UnequipWeapon(WeaponType type);

  void Kill() override;
  bool isBig() { return mIsBig; };
  bool isInvulnerable() { return mIsInvulnerable; };
  class Aim* GetAim() const { return mAimer; }

  uint8_t GetCurrentLvl() const { return mCurrentLvl; }
  uint32_t GetCurrentXP() const { return mCurrentXp; }
  void AddXP(uint32_t amount);
  uint32_t GetMaxXP() const;

 private:
  void ManageAnimations();

  float mForwardSpeed;
  float mJumpSpeed;
  bool mIsRunning;
  bool mIsDead;
  bool mIsBig = false;
  bool mIsInvulnerable;
  float mInvulnerabilityTimer;
  const float INVULNERABILITY_DURATION = 1.5f;
  float mGrowTimer = 0.0f;
  class RigidBodyComponent* mRigidBodyComponent;
  class AnimatorComponent* mDrawComponent;
  class AABBColliderComponent* mColliderComponent;
  class ParticleSystemComponent* mFireBalls;

  float mFireBallCooldown = 1.0f;

  uint8_t mCurrentLvl{0};
  uint32_t mCurrentXp{0};

  class Aim* mAimer;
};
