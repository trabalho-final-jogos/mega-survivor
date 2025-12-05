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
      mCooldownTimer(0.0f),
      mCooldownTime(2.0),
      mDamage(0.0){
  // 1. Cria o seu próprio pool
  mProjectilePool = new ProjectilePoolComponent();

  for (int i = 0; i < POOL_SIZE; i++) {
    auto bullet =
        new IceProjectile(mOwner->GetGame(), PARTICLE_WIDTH, PARTICLE_HEIGHT);
    mProjectilePool->AddObjectToPool(bullet);
  }

  Player* player = static_cast<Player*>(mOwner);
  mAim = player->GetAim();

  LevelUp();
}

IceGun::~IceGun() {
  // Deleta o pool que esta arma possui
  delete mProjectilePool;
}

void IceGun::OnUpdate(float deltaTime) {
  if (mCooldownTimer > 0.0f) {
    mCooldownTimer -= deltaTime;
  }

  if (mCooldownTimer <= 0.0f) {
    FireShot();
    mCooldownTimer = mCooldownTime;
  }
}

void IceGun::LevelUp() {
  mLevel++; // Sobe o nível
  SDL_Log("IceGun subiu para o Nível %d!", mLevel);
  switch(mLevel)
  {
    case 1:
      mCooldownTime = 1.8f;
      mDamage = 8.0f;
      mAreaScale = 1.0f; // 100% do tamanho

      break;
    case 2:
      mCooldownTime = 1.5f;
      mDamage = 10.0f;
      mAreaScale = 1.0f;
      break;
    case 3:
      mCooldownTime = 1.2f;
      mDamage = 12.0f;
      mAreaScale = 1.2f;
      break;
  }
}
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
             finalVelocity, mDamage, mAreaScale);
    // /p->GetComponent<RigidBodyComponent>()->SetVelocity(finalVelocity);

    // 4d. Incrementa o ângulo para o próximo tiro
    currentAngle += angleIncrement;
  }
}
