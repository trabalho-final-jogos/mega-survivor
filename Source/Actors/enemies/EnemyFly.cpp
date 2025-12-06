
#include "EnemyFly.h"

#include "../../Components/Drawing/AnimatorComponent.h"
#include "../../Components/Physics/AABBColliderComponent.h"
#include "../../Components/Physics/RigidBodyComponent.h"
#include "../../Game.h"
#include "../Player.h"

EnemyFly::EnemyFly(Game* game, float forwardSpeed, float deathTime)
		: Enemy(game, 10, 25),
			mIsDying(false),
			mForwardSpeed(forwardSpeed)

{
	mDrawComponent = new AnimatorComponent(
			this, "../Assets/Sprites/enemy2/enemy2.png",
			"../Assets/Sprites/enemy2/enemy2.json", Game::TILE_SIZE, Game::TILE_SIZE);
	mDrawComponent->SetEnabled(true);
	mDrawComponent->SetVisible(true);
	mDrawComponent->AddAnimation("walk", std::vector<int>{0, 1,2 ,1});
	mDrawComponent->SetAnimation("walk");
	mDrawComponent->SetAnimFPS(10.0f);

	mRigidBody = new RigidBodyComponent(this);
	mRigidBody->SetVelocity(Vector2(-forwardSpeed, 0.0f));
	mColliderComponent = new class AABBColliderComponent(
			this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Enemy,
			false);
}

void EnemyFly::Kill() {
	mIsDying = true;
	mRigidBody->SetEnabled(false);
	mColliderComponent->SetEnabled(false);
}

void EnemyFly::OnUpdate(float deltaTime) {
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

void EnemyFly::SetStats(float health, float speed) {
	mHealth = health;
	mForwardSpeed = speed;
}