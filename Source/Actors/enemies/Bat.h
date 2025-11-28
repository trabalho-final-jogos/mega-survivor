#pragma once
#include "../Actor.h"

class Bat : public Actor
{
public:
	Bat(class Game* game);
	void OnUpdate(float deltaTime) override;
	void OnHorizontalCollision(const float minOverlap, class AABBColliderComponent* other) override;
	void OnVerticalCollision(const float minOverlap, class AABBColliderComponent* other) override;

	// Define a velocidade fixa (chamado pelo Spawner)
	void SetFixedVelocity(const Vector2& velocity);

	void SetStats(float health);

private:
	int mHealth;
	class RigidBodyComponent* mRigidBody;
	class AnimatorComponent* mDrawComponent; // Ou SpriteComponent
};