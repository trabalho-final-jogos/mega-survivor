#include "Enemy.h"

#include "../Game.h"
#include "XPGem.h"

Enemy::Enemy(Game* game, int health, uint16_t xpDrop)
    : Actor(game),
      mHealth(health),
      mXPDrop(xpDrop),
      mRigidBody(nullptr),
      mDrawComponent(nullptr),
      mColliderComponent(nullptr) {}

void Enemy::TakeDamage(float damage) {
  mGame->GetAudioSystem()->PlaySound("hitHurt.wav");

  SDL_Log("Damge %f", damage);
  SDL_Log("mhealt %f", mHealth);

  mHealth -= damage;
  if (mHealth <= 0) {
    Kill();
    new XPGem(GetGame(), GetPosition(), mXPDrop);
  }
}

void Enemy::OnHorizontalCollision(const float minOverlap,
                                  AABBColliderComponent* other) {
  if (other->GetLayer() == ColliderLayer::Player) {
    if (mGame->GetClockTime() >= mLastHitTime + ATTACK_DELAY) {
      Player* player = GetGame()->GetPlayer();

      if (player) {
        player->TakeDamage(mDamage);
      }

      mLastHitTime = mGame->GetClockTime();
    }
  }
}

void Enemy::OnVerticalCollision(const float minOverlap,
                                AABBColliderComponent* other) {
  OnHorizontalCollision(minOverlap, other);
}