//
// Created by filipe on 11/1/25.
//
// BoomerangWeapon.cpp
#include "BoomerangGun.h"
#include "../../../Components/ProjectilePoolComponent.h"
#include "BoomerangProjectile.h" // <-- Inclui o novo projétil
#include "../../Actor.h"
#include "../../../Game.h"
#include "../../../Actors/Player.h"
#include "../../../Actors/Aim.h"
#include "../../../Components/Physics/RigidBodyComponent.h"

BoomerangGun::BoomerangGun(Actor* owner, int updateOrder)
    : WeaponComponent(owner, WeaponType::BoomerangGun,updateOrder)
    , mProjectilePool(nullptr)
    , mAimer(nullptr)
    , mCooldownTimer(0.0f)
{
  // Cria o pool
  mProjectilePool = new ProjectilePoolComponent();

  // Preenche o pool com o PROJÉTIL DE BUMERANGUE
  for (int i = 0; i < POOL_SIZE; i++)
  {
    auto b = new BoomerangProjectile(mOwner->GetGame(), PARTICLE_WIDTH, PARTICLE_HEIGHT);
    mProjectilePool->AddObjectToPool(b);
  }

  Player* player = static_cast<Player*>(mOwner);
  mAimer = player->GetAim();

}

void BoomerangGun::OnUpdate(float deltaTime)
{
  // Atualiza o cooldown
  if (mCooldownTimer > 0.0f)
  {
    mCooldownTimer -= deltaTime;
  }

  // Dispara automaticamente (estilo Vampire Survivors)
  if (mCooldownTimer <= 0.0f)
  {
    FireShot();
    mCooldownTimer = COOLDOWN_TIME; // Reseta o cooldown
  }
}

void BoomerangGun::LevelUp() {
}

void BoomerangGun::FireShot()
{
  if (!mAimer) { return; }

  Projectile* p = mProjectilePool->GetDeadObject();
  if (p)
  {
    // "Acorda" o bumerangue na posição do jogador
    // Passa 'mOwner' (o Player) para o bumerangue saber para quem voltar
    // Passa velocidade Zero, pois o bumerangue calcula a própria velocidade
    p->Awake(mOwner, mOwner->GetPosition(),
             mOwner->GetRotation(), PROJECTILE_LIFETIME, Vector2::Zero);
  }
}
