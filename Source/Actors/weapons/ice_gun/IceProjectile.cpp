// IceProjectile.cpp
#include "IceProjectile.h"
#include "../../../Components/Drawing/AnimatorComponent.h"  // <-- Usa um Sprite, não um Animator
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../../Game.h"
#include "../../../Math.h"  // Para ToRadians e Vector2::Perpendicular
#include "../../Aim.h"      // Necessário para pegar a posição da Mira
#include "../../Enemy.h"
#include "../../Player.h"  // Necessário para pegar o Ator do Player e a Mira

IceProjectile::IceProjectile(Game* game, int width, int height)
    : Projectile(
          game,
          width,
          height)  // Chama o construtor da base (que carrega o spritesheet)
{
  // O construtor de 'Projectile' já carregou o spritesheet enorme.
  // Nós apenas definimos a animação para ESTE projétil.
  if (mDrawComponent) {
    mDrawComponent->AddAnimation("fly_ice", std::vector<int>{9});
  }
}

void IceProjectile::Awake(Actor* owner,
                          const Vector2& position,
                          float rotation,
                          float lifetime,
                          const Vector2& velocity,
                          float damage,
                          float areaScale) {
  Projectile::Awake(owner, position, rotation, lifetime, velocity, damage,
                    areaScale);

  GetGame()->GetAudioSystem()->PlaySound("shot2.wav");

  if (mDrawComponent) {
    mDrawComponent->SetAnimation("fly_ice");
  }
}

void IceProjectile::OnHorizontalCollision(const float minOverlap,
                                          AABBColliderComponent* other) {
  ColliderLayer otherLayer = other->GetLayer();

  if (otherLayer == ColliderLayer::Blocks) {
    Kill();  // Desativa (retorna ao pool)
  } else if (otherLayer == ColliderLayer::Enemy) {
    Actor* enemyActor = other->GetOwner();
    Enemy* enemy = dynamic_cast<Enemy*>(enemyActor);

    if (enemy) {
      enemy->TakeDamage(this->GetDamage());
    }
    Kill();
  }
}

void IceProjectile::OnVerticalCollision(const float minOverlap,
                                        AABBColliderComponent* other) {
  // Em top-down, a lógica é idêntica à horizontal
  OnHorizontalCollision(minOverlap, other);
}