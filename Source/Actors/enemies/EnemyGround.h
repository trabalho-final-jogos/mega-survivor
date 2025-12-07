#pragma once

#include "../Enemy.h"

class EnemyGround : public Enemy {
public:
	explicit EnemyGround(Game* game,
									float forwardSpeed = 80.0f,
									float deathTime = 0.5f);

	void OnUpdate(float deltaTime) override;

	void Kill() override;

	void SetStats(float health, float speed);

	// Getters opcionais (se precisar ler a velocidade depois)
	float GetSpeed() const { return mForwardSpeed; }

private:
	bool mIsDying;
	float mForwardSpeed;

	float mMoveSpeed;
};