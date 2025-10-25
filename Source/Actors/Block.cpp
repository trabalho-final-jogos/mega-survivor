//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Block.h"
#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Components/Physics/AABBColliderComponent.h"

Block::Block(Game* game, const std::string &texturePath, std::string name)
        :Actor(game)
    , mOriginalPosition(Vector2::Zero)
    , mIsBouncing(false)
    , mBounceTimer(0.0f)
    , mCanBounce(false)
{
        this->name = name;
        if (this->name == "Block B") {


            mCanBounce =  true;
        }

        SetScale(Vector2(Game::TILE_SIZE, Game::TILE_SIZE));

        auto mdraw  = new AnimatorComponent(
            this,
            texturePath,
            "",
            Game::TILE_SIZE,
            Game::TILE_SIZE
        );
    mdraw->SetFlipHorizontal(false);

    new AABBColliderComponent(
        this,
        0,
        0,
        Game::TILE_SIZE,
        Game::TILE_SIZE,
        ColliderLayer::Blocks,
        true
    );

}

void Block::StartBounce()
{
    if (!mCanBounce || mIsBouncing)
    {
        return;
    }

    mIsBouncing = true;
    mBounceTimer = 0.0f;
    mOriginalPosition = GetPosition();
}
void Block::OnUpdate(float deltaTime)
{
    if (!mIsBouncing)
    {
        return;
    }
    mBounceTimer += deltaTime;

    const float BOUNCE_HEIGHT = 6.0f;
    const float BOUNCE_DURATION = 0.25f;
    const float halfDuration = BOUNCE_DURATION / 2.0f;

    float offsetY = 0.0f;

    if (mBounceTimer < halfDuration)
    {
        offsetY = Math::Lerp(0.0f, -BOUNCE_HEIGHT, mBounceTimer / halfDuration);
    }
    else
    {
        offsetY = Math::Lerp(-BOUNCE_HEIGHT, 0.0f, (mBounceTimer - halfDuration) / halfDuration);
    }

    SetPosition(Vector2(mOriginalPosition.x, mOriginalPosition.y + offsetY));

    if (mBounceTimer >= BOUNCE_DURATION)
    {
        mIsBouncing = false;
        mBounceTimer = 0.0f;
        SetPosition(mOriginalPosition);
    }
}