//
// Created by filipe on 11/1/25.
//
// BoomerangWeapon.cpp
#include "BoomerangGun.h"
#include "../../../Actors/Aim.h"
#include "../../../Actors/Player.h"
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../../Components/ProjectilePoolComponent.h"
#include "../../../Game.h"
#include "../../Actor.h"
#include "BoomerangProjectile.h"  // <-- Inclui o novo projétil

BoomerangGun::BoomerangGun(Actor* owner, int updateOrder)
    : WeaponComponent(owner, WeaponType::BoomerangGun, updateOrder),
      mProjectilePool(nullptr),
      mAimer(nullptr),
      mCooldownTimer(0.0f),
      mCooldownTime(1.8f){
  // Cria o pool
  mProjectilePool = new ProjectilePoolComponent();

  // Preenche o pool com o PROJÉTIL DE BUMERANGUE
  for (int i = 0; i < POOL_SIZE; i++) {
    auto b = new BoomerangProjectile(mOwner->GetGame(), PARTICLE_WIDTH,
                                     PARTICLE_HEIGHT);
    mProjectilePool->AddObjectToPool(b);
  }

  Player* player = static_cast<Player*>(mOwner);
  mAimer = player->GetAim();
  LevelUp();
}

void BoomerangGun::OnUpdate(float deltaTime) {
  // Atualiza o cooldown
  if (mCooldownTimer > 0.0f) {
    mCooldownTimer -= deltaTime;
  }

  // Dispara automaticamente (estilo Vampire Survivors)
  if (mCooldownTimer <= 0.0f) {
    FireShot();
    mCooldownTimer = mCooldownTime;  // Reseta o cooldown
  }
}

void BoomerangGun::LevelUp() {
  mLevel++; // Sobe o nível
  SDL_Log("BoomerangGun subiu para o Nível %d!", mLevel);
  switch(mLevel)
  {
    case 1:
      mCooldownTime = 1.4f;
      mDamage = 20.0f;
      mAreaScale = 1.3f; // 100% do tamanho

      break;
    case 2:
      mCooldownTime = 1.2f;
      mDamage = 20.0f;
      mAreaScale = 1.0f;
      break;
    case 3:
      mCooldownTime = 1.0f;
      mDamage = 22.0f;
      mAreaScale = 1.2f;
      break;
  }
}

void BoomerangGun::FireShot() {
  if (!mAimer) {
    return;
  }

  Projectile* p = mProjectilePool->GetDeadObject();
  if (p) {
    // "Acorda" o bumerangue na posição do jogador
    // Passa 'mOwner' (o Player) para o bumerangue saber para quem voltar
    // Passa velocidade Zero, pois o bumerangue calcula a própria velocidade
    p->Awake(mOwner, mOwner->GetPosition(), mOwner->GetRotation(),
             PROJECTILE_LIFETIME, Vector2::Zero, mDamage, mAreaScale);
  }
}
