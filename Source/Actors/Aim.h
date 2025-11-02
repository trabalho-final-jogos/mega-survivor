// Coin.h
#pragma once
#include "Actor.h"
#include "Player.h"
#include "../Math.h"

class Aim : public Actor
{
public:
    Aim(class Game* game, class Player* owner);
    void OnUpdate(float deltaTime) override;

private:
    class AnimatorComponent* mSprite;
    Player* mOwnerPlayer;
    const float AIMER_RADIUS = 32.0f;
};