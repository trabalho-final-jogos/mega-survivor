// Fireball.cpp
#include "Bullet.h"
#include "../../../Components/Drawing/AnimatorComponent.h"
#include "../../../Components/ParticleSystemComponent.h"
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../../Game.h"
#include "../../Enemy.h"

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
  // Se bater num Inimigo
  if (otherLayer == ColliderLayer::Enemy)
  {
    // 1. Identifica o Inimigo
    Actor* enemyActor = other->GetOwner();

    // Tenta fazer o cast para a classe Enemy (ou Goomba) para chamar TakeDamage
    // (Se Enemy herda de Actor e Actor tem TakeDamage virtual, não precisa de cast)
    Enemy* enemy = dynamic_cast<Enemy*>(enemyActor);

    if (enemy)
    {
      // 2. APLICA O DANO DIRETAMENTE
      // A bala é a "agressora", ela sabe quanto dano tem.
      enemy->TakeDamage(this->GetDamage());
    }

    // 3. Destrói a bala
    // Como a bala processou tudo, ela pode morrer em paz.
    Kill();
  }
  // Se bater em Paredes
  else if (otherLayer == ColliderLayer::Blocks)
  {
    Kill();
  }
}

void Bullet::OnVerticalCollision(const float minOverlap,
                                 AABBColliderComponent* other) {
  // No top-down, a lógica vertical é idêntica à horizontal
  OnHorizontalCollision(minOverlap, other);
}