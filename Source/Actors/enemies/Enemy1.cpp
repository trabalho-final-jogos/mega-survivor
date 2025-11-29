//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Enemy1.h"

#include "../../Components/Drawing/AnimatorComponent.h"
#include "../../Components/Physics/AABBColliderComponent.h"
#include "../../Components/Physics/RigidBodyComponent.h"
#include "../../Game.h"
#include "../Player.h"

Enemy1::Enemy1(Game* game, float forwardSpeed, float deathTime)
    : Enemy(game, 10, 15),
      mIsDying(false),
      mForwardSpeed(forwardSpeed)

{
  mDrawComponent = new AnimatorComponent(
      this, "../Assets/Sprites/enemy1/enemy1.png",
      "../Assets/Sprites/enemy1/enemy1.json", Game::TILE_SIZE, Game::TILE_SIZE);
  mDrawComponent->SetEnabled(true);
  mDrawComponent->SetVisible(true);
  mDrawComponent->AddAnimation("walk", std::vector<int>{1, 2});
  mDrawComponent->AddAnimation("idle", std::vector<int>{0});
  mDrawComponent->SetAnimation("walk");
  mDrawComponent->SetAnimFPS(3.0f);

  mRigidBody = new RigidBodyComponent(this);
  mRigidBody->SetVelocity(Vector2(-forwardSpeed, 0.0f));
  mColliderComponent = new class AABBColliderComponent(
      this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Enemy,
      false);
}

void Enemy1::Kill() {
  mIsDying = true;
  //  mDrawComponent->SetAnimation("dead");
  mRigidBody->SetEnabled(false);
  mColliderComponent->SetEnabled(false);
}

void Enemy1::OnUpdate(float deltaTime) {
  if (mIsDying) {
    mState = ActorState::Destroy;
  }

  const Player* player = GetGame()->GetPlayer();

  if (player) {
    Vector2 playerPos = player->GetPosition();
    Vector2 myPos = GetPosition();

    Vector2 direction = playerPos - myPos;

    if (direction.LengthSq() > 0.0f) {
      direction.Normalize();
    }

    if (mRigidBody) {
      float speed = mForwardSpeed;

      mRigidBody->SetVelocity(direction * speed);
    }
  }
}

void Enemy1::OnHorizontalCollision(const float minOverlap,
                                   AABBColliderComponent* other) {
  if (other->GetLayer() == ColliderLayer::Player) {
    Actor* player = other->GetOwner();
    Player* mario = dynamic_cast<Player*>(player);
    if (!mario->isInvulnerable()) {
      if (mario->isBig()) {
        mario->Shrink();
      } else {
        mario->Kill();
      }
    }
  }

  if (other->GetLayer() == ColliderLayer::Blocks ||
      other->GetLayer() == ColliderLayer::Enemy) {
    Vector2 currentVelocity = mRigidBody->GetVelocity();

    if (minOverlap < 0) {
      mRigidBody->SetVelocity(Vector2(-mForwardSpeed, currentVelocity.y));
    } else {
      mRigidBody->SetVelocity(Vector2(mForwardSpeed, currentVelocity.y));
    }
  }
}

void Enemy1::OnVerticalCollision(const float minOverlap,
                                 AABBColliderComponent* other) {
  ColliderLayer otherLayer = other->GetLayer();
  if (otherLayer == ColliderLayer::Player) {
    Actor* player = other->GetOwner();

    Player* mario = dynamic_cast<Player*>(player);
    if (!mario->isInvulnerable()) {
      if (minOverlap < 0) {
        if (mario->isBig()) {
          mario->Shrink();
        } else {
          mario->Kill();
        }
      }
    }
    SDL_Log("Goomba::OnVerticalCollision %f", minOverlap);
  }
}

void Enemy1::SetStats(float health, float speed) {
  mHealth = health;
  mForwardSpeed = speed;
}