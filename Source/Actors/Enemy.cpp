#include "Enemy.h"

#include "../Game.h"

Enemy::Enemy(Game* game, int health, uint16_t xpDrop)
    : Actor(game), mHealth(health), mXPDrop(xpDrop) {}

void Enemy::TakeDamage(int damage) {
  mHealth -= damage;
  if (mHealth <= 0) {
    Kill();
  }
}
