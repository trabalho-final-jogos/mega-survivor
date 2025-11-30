// IceGun.cpp
#include "IceGun.h"
#include "../../../Components/Drawing/AnimatorComponent.h"  // <-- Usa um Sprite, não um Animator
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../../Components/ProjectilePoolComponent.h"
#include "../../../Game.h"
#include "../../../Math.h"  // Para ToRadians e Vector2::Perpendicular
#include "../../Aim.h"      // Necessário para pegar a posição da Mira
#include "../../Player.h"   // Necessário para pegar o Ator do Player e a Mira
#include "IceProjectile.h"

IceGun::IceGun(Actor* owner, int updateOrder)
    : WeaponComponent(owner, WeaponType::IceGun, updateOrder),
      mProjectilePool(nullptr),
      mAim(nullptr),
      mCooldownTimer(0.0f) {
  // 1. Cria o seu próprio pool
  mProjectilePool = new ProjectilePoolComponent();

  // 2. Preenche o pool com o PROJÉTIL DE GELO
  for (int i = 0; i < POOL_SIZE; i++) {
    auto bullet =
        new IceProjectile(mOwner->GetGame(), PARTICLE_WIDTH, PARTICLE_HEIGHT);
    mProjectilePool->AddObjectToPool(bullet);
  }

  // 3. Cache da Mira (Aim)
  Player* player = static_cast<Player*>(mOwner);
  mAim = player->GetAim();
}

IceGun::~IceGun() {
  // Deleta o pool que esta arma possui
  delete mProjectilePool;
}

void IceGun::OnUpdate(float deltaTime) {
  // Atualiza o cooldown
  if (mCooldownTimer > 0.0f) {
    mCooldownTimer -= deltaTime;
  }

  // Dispara automaticamente (estilo Vampire Survivors)
  if (mCooldownTimer <= 0.0f) {
    FireShot();
    mCooldownTimer = COOLDOWN_TIME;
  }
}

void IceGun::LevelUp() {}

void IceGun::FireShot() {
  if (!mAim) {
    return;
  }

  Player* player = static_cast<Player*>(mOwner);
  Vector2 playerPos = player->GetPosition();
  Vector2 aimerPos = mAim->GetPosition();

  // 1. Pega a direção central (para a mira)
  Vector2 centerDirection = (aimerPos - playerPos);
  if (centerDirection.LengthSq() == 0.0f) {
    centerDirection = Vector2(1.0f, 0.0f);
  }
  centerDirection.Normalize();

  // 2. Pega a velocidade herdada do jogador
  Vector2 playerVelocity =
      player->GetComponent<RigidBodyComponent>()->GetVelocity();

  // 3. Calcula os ângulos do leque
  float totalSpreadRad = Math::ToRadians(SPREAD_ANGLE);
  float angleIncrement =
      (NUM_PROJECTILES > 1) ? (totalSpreadRad / (NUM_PROJECTILES - 1)) : 0.0f;
  float currentAngle = (NUM_PROJECTILES > 1) ? (-totalSpreadRad / 2.0f)
                                             : 0.0f;  // Começa em -15 graus

  // 4. Dispara todos os 5 projéteis
  for (int i = 0; i < NUM_PROJECTILES; i++) {
    // Pega um projétil inativo do pool
    Projectile* p = mProjectilePool->GetDeadObject();
    if (!p) {
      break;
    }  // Parar se o pool estiver vazio

    // 4a. Calcula a direção individual deste projétil
    Vector2 shotDirection = centerDirection.Rotate(currentAngle);

    // 4b. Calcula a velocidade final (herdada + projétil)
    Vector2 finalVelocity = (shotDirection * PROJECTILE_SPEED) + playerVelocity;

    // 4c. "Acorda" o projétil
    p->Awake(mOwner, playerPos, mOwner->GetRotation(), PROJECTILE_LIFETIME,
             finalVelocity);
    // /p->GetComponent<RigidBodyComponent>()->SetVelocity(finalVelocity);

    // 4d. Incrementa o ângulo para o próximo tiro
    currentAngle += angleIncrement;
  }
}
