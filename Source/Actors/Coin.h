// Coin.h
#pragma once
#include "Actor.h"
#include "../Math.h"

class Coin : public Actor
{
public:
    Coin(class Game* game, const Vector2& initialPos);
    void OnUpdate(float deltaTime) override;

private:
    class AnimatorComponent* mSprite;

    float mLifeTimer;
    float mVerticalSpeed;

    const float INITIAL_LIFE = 0.5f;
    const float UP_SPEED = -150.0f;
    const float GRAVITY = 400.0f;
};