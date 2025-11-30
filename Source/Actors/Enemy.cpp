#include "Enemy.h"

#include "../Game.h"
#include "XPGem.h"

Enemy::Enemy(Game* game, int health, uint16_t xpDrop)
    : Actor(game), mHealth(health), mXPDrop(xpDrop) {}

void Enemy::TakeDamage(int damage) {
  mHealth -= damage;
  if (mHealth <= 0) {
    Kill();
    new XPGem(GetGame(), GetPosition(), mXPDrop);
  }
}

void Enemy::OnHorizontalCollision(const float minOverlap,
                                  AABBColliderComponent* other) {
  if (other->GetLayer() == ColliderLayer::PlayerProjectile) {
    Projectile* projectile = dynamic_cast<Projectile*>(other->GetOwner());
    if (projectile) {
      TakeDamage(projectile->GetDamage());
    }
  }
}

void Enemy::OnVerticalCollision(const float minOverlap,
                                AABBColliderComponent* other) {
  OnHorizontalCollision(minOverlap, other);
}