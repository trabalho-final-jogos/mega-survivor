// Fireball.cpp
#include "Bullet.h"
#include "../../../Components/Drawing/AnimatorComponent.h"
#include "../../../Components/ParticleSystemComponent.h"
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../../Game.h"

Bullet::Bullet(Game* game, int width, int height)
    : Projectile(game,
                 width,
                 height)  // Chama o construtor da base (Projectile)
{
  if (mDrawComponent) {
    mDrawComponent->AddAnimation("bullet", std::vector<int>{3});
    mDrawComponent->SetAnimation("bullet");
  }

  // 3. Define a escala do Ator para o tamanho do sprite/bala
  SetScale(Vector2(static_cast<float>(width), static_cast<float>(height)));

  // 4. Garante que o novo componente de desenho comece invisível
  //    (O estado Paused/Inactive já deve cuidar disso, mas é uma garantia)
  mDrawComponent->SetVisible(false);
}

void Bullet::OnHorizontalCollision(const float minOverlap,
                                   AABBColliderComponent* other) {
  ColliderLayer otherLayer = other->GetLayer();

  // Lógica de dano da Bala: Atinge Blocos E Inimigos
  if (otherLayer == ColliderLayer::Blocks ||
      otherLayer == ColliderLayer::Enemy) {
    // Desativa a si mesmo (retorna ao pool através do Kill() da base)
    Kill();
  }

  // (Ignora colisões com Player, PowerUps, etc.)
}

void Bullet::OnVerticalCollision(const float minOverlap,
                                 AABBColliderComponent* other) {
  // No top-down, a lógica vertical é idêntica à horizontal
  OnHorizontalCollision(minOverlap, other);
}