//
// Created by Lucas N. Ferreira on 08/09/23.
//

#pragma once
#include "../Components/Upgrades/UpgradeComponent.h"
#include "Actor.h"
#include "Aim.h"
#include "weapons/WeaponType.h"

enum PlayerChar { MEGAMAN, PROTOMAN, BASS, LAST_CHAR };

constexpr float PLAYER_BASE_SPEED{200.0f};
const float INVULNERABILITY_DURATION = 1.5f;
constexpr uint kCharCount = static_cast<uint>(PlayerChar::LAST_CHAR);

struct CharInfo {
  PlayerChar playerChar;
  std::string_view charName;
  WeaponType charWeapon;
};

namespace CharacterDB {
inline constexpr CharInfo INFOS[] = {
    {PlayerChar::MEGAMAN, "Megaman", WeaponType::MainGun},
    {PlayerChar::PROTOMAN, "Protoman", WeaponType::IceGun},
    {PlayerChar::BASS, "Bass", WeaponType::LaserGun}};

inline const CharInfo& Get(PlayerChar character) {
  return INFOS[static_cast<int>(character)];
}
}  // namespace CharacterDB

class Player : public Actor {
 public:
  explicit Player(Game* game,
                  CharInfo pcharInfo,
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

  void SetCharInfo(CharInfo* charInfo);

  void Kill() override;
  bool isBig() { return mIsBig; };
  bool isInvulnerable() { return mIsInvulnerable; };
  class Aim* GetAim() const { return mAimer; }

  uint8_t GetCurrentLvl() const { return mCurrentLvl; }
  uint32_t GetCurrentXP() const { return mCurrentXp; }
  uint32_t GetCurrentHP() const { return mCurrentHP; }
  void AddXP(uint32_t amount);
  uint32_t GetMaxXP() const;
  uint32_t GetMaxHP() const { return mMaxHP; }
  void TakeDamage(uint32_t damage);
  void HealDamage(uint32_t heal);

  // In-game upgrades support
  void ApplyRunUpgrade(
      Stats type,
      float amount = 1.0f);  // Amount is generic, logic can be inside or passed

  class UpgradeComponent* GetUpgradeComponent() const {
    return mUpgradeComponent;
  }

  // Stat Getters for Weapons/Gameplay
  float GetDamageMultiplier() const;
  float GetAreaMultiplier() const;
  float GetProjectileSpeedMultiplier() const;  // Example mapping
  int GetAdditionalProjectiles() const;
  float GetCooldownReduction() const;  // Could map to Speed or separate stat
  float GetLuck() const;

 private:
  void ManageAnimations();

  float mForwardSpeed;
  float mJumpSpeed;
  bool mIsRunning;
  bool mIsDead;
  bool mIsBig = false;
  bool mIsInvulnerable;
  float mInvulnerabilityTimer;

  float mGrowTimer = 0.0f;
  class RigidBodyComponent* mRigidBodyComponent;
  class AnimatorComponent* mDrawComponent;
  class AABBColliderComponent* mColliderComponent;
  class ParticleSystemComponent* mFireBalls;

  float mFireBallCooldown = 1.0f;

  uint8_t mCurrentLvl{0};
  uint32_t mCurrentXp{0};
  uint32_t mMaxHP{20};
  uint32_t mCurrentHP{mMaxHP};
  float mHealthRegenTimer{0.0f};

  CharInfo* mCharInfo;

  class Aim* mAimer;
  class UpgradeComponent* mUpgradeComponent;
};
