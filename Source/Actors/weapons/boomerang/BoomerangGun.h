// BoomerangWeapon.h
#pragma once
#include "../../../Components/ProjectilePoolComponent.h"
#include "../../../Components/WeaponComponent.h"
#include "../../Aim.h"

class BoomerangGun : public WeaponComponent {
 public:
  BoomerangGun(class Actor* owner, int updateOrder = 100);
  void OnUpdate(float deltaTime) override;
  void LevelUp() override;

 private:
  void FireShot();

  ProjectilePoolComponent* mProjectilePool;
  Aim* mAimer;

  // O bumerangue não tem rajada, só um cooldown longo
  const float COOLDOWN_TIME = 1.5f;
  const float PROJECTILE_LIFETIME = 10.0f;  // Tempo máximo de vida
  const int PARTICLE_WIDTH = 16;
  const int PARTICLE_HEIGHT = 16;
  const int POOL_SIZE = 15;  // Só precisamos de alguns

  float mCooldownTimer;
};