// Mushroom.cpp
#include "Mushroom.h"
#include "Player.h"
#include "../Game.h"
#include "../Components/Physics/RigidBodyComponent.h"
#include "../Components/Physics/AABBColliderComponent.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include <algorithm>

Mushroom::Mushroom(Game* game, const Vector2& initialPos)
    : Actor(game)
    , mRigidBody(nullptr)
    , mWalkSpeed(40.0f)
    , mIsPoppingUp(true)
    , mPopUpDistance(0.0f)
    , mPopStartY(initialPos.y)
    , mPopTargetY(initialPos.y - POP_UP_TARGET)
{
    SetPosition(initialPos);
    SetScale(Vector2(Game::TILE_SIZE, Game::TILE_SIZE));

    auto mdraw  = new AnimatorComponent(
            this,
            "../Assets/Sprites/Collectables/Mushroom.png",
            "",
            Game::TILE_SIZE,
            Game::TILE_SIZE
        );

    mRigidBody = new RigidBodyComponent(this);
    mRigidBody->SetApplyGravity(false);

    new AABBColliderComponent(
        this,
        0, 0,
        Game::TILE_SIZE, Game::TILE_SIZE,
        ColliderLayer::PowerUp,
        false
    );
}

void Mushroom::OnUpdate(float deltaTime)
{
    Actor::OnUpdate(deltaTime);

    if (mIsPoppingUp)
    {
        float moveAmount = POP_UP_SPEED * deltaTime;
        Vector2 pos = GetPosition();
        float newY = pos.y - moveAmount;

        if (newY <= mPopTargetY)
        {
            newY = mPopTargetY;
            SetPosition(Vector2(pos.x, newY));

            mIsPoppingUp = false;

            if (mRigidBody)
            {
                mRigidBody->SetVelocity(Vector2(-mWalkSpeed, 0.0f));
                mRigidBody->SetApplyGravity(true);
            }
        }
        else
        {
            SetPosition(Vector2(pos.x, newY));
        }

        return;
    }

    float bottomOfLevel = Game::LEVEL_HEIGHT * Game::TILE_SIZE;
    if (GetPosition().y > (bottomOfLevel + (Game::TILE_SIZE * 2)))
    {
        SetState(ActorState::Destroy);
    }
}

void Mushroom::OnHorizontalCollision(float minOverlap, AABBColliderComponent* other)
{
    if (mIsPoppingUp) return;

    ColliderLayer otherLayer = other->GetLayer();

    if (otherLayer == ColliderLayer::Blocks)
    {
        if (mRigidBody)
        {
            Vector2 vel = mRigidBody->GetVelocity();

            if (minOverlap > 0.0f) { vel.x = mWalkSpeed; }
            else if (minOverlap < 0.0f) { vel.x = -mWalkSpeed; }

            mRigidBody->SetVelocity(vel);
        }
    }
    if (auto mario = dynamic_cast<Player*>(other->GetOwner()))
    {
        mario->Grow();
        SetState(ActorState::Destroy);
        SDL_Log("Mario pegou o cogumelo H!");
        return;
    }
}

void Mushroom::OnVerticalCollision(float minOverlap, AABBColliderComponent *other) {
    if (auto mario = dynamic_cast<Player*>(other->GetOwner()))
    {
        mario->Grow();
        SetState(ActorState::Destroy);
        SDL_Log(" Mario pegou o cogumelo! V");
        return;
    }
}
