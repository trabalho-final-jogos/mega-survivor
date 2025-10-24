// Coin.cpp
#include "Coin.h"
#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"

Coin::Coin(Game* game, const Vector2& initialPos)
    : Actor(game)
    , mSprite(nullptr)
    , mLifeTimer(INITIAL_LIFE)
    , mVerticalSpeed(UP_SPEED)
{
    SetPosition(initialPos);
    // A moeda pode ter um tamanho diferente, ajuste se necessário
    SetScale(Vector2(Game::TILE_SIZE * 0.8f, Game::TILE_SIZE * 0.8f));

    // Cria o componente de sprite
    mSprite = new AnimatorComponent(
            this,
            "../Assets/Sprites/Collectables/Coin.png",
            "",
            Game::TILE_SIZE,
            Game::TILE_SIZE
        );
}

void Coin::OnUpdate(float deltaTime)
{
    Actor::OnUpdate(deltaTime);

    mLifeTimer -= deltaTime;

    if (mLifeTimer <= 0.0f)
    {
        SetState(ActorState::Destroy);
        return; 
    }

    Vector2 pos = GetPosition();
    pos.y += mVerticalSpeed * deltaTime;
    SetPosition(pos);

    mVerticalSpeed += GRAVITY * deltaTime;
}