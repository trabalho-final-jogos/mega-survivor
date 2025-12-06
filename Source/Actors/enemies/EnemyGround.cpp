//
// Created by filipe on 12/5/25.
//

#include "EnemyGround.h"

#include "../../Components/Drawing/AnimatorComponent.h"
#include "../../Components/Physics/AABBColliderComponent.h"
#include "../../Components/Physics/RigidBodyComponent.h"
#include "../../Game.h"
#include "../Player.h"

EnemyGround::EnemyGround(Game* game, float forwardSpeed, float deathTime)
		: Enemy(game, 10, 25),
			mIsDying(false),
			mForwardSpeed(forwardSpeed)

{
	mDrawComponent = new AnimatorComponent(
			this, "../Assets/Sprites/enemy3/enemy3.png",
			"../Assets/Sprites/enemy3/enemy3.json", Game::TILE_SIZE, Game::TILE_SIZE);
	mDrawComponent->SetEnabled(true);
	mDrawComponent->SetVisible(true);
	mDrawComponent->AddAnimation("walk", std::vector<int>{0, 1});
	mDrawComponent->SetAnimation("walk");
	mDrawComponent->SetAnimFPS(6.0f);

	mRigidBody = new RigidBodyComponent(this);
	mRigidBody->SetVelocity(Vector2(-forwardSpeed, 0.0f));
	mColliderComponent = new class AABBColliderComponent(
			this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Enemy,
			false);
}

void EnemyGround::Kill() {
	mIsDying = true;
	mRigidBody->SetEnabled(false);
	mColliderComponent->SetEnabled(false);
}

void EnemyGround::OnUpdate(float deltaTime) {
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

void EnemyGround::SetStats(float health, float speed) {
	mHealth = health;
	mForwardSpeed = speed;
}