#include "QuestionBlock.h"

#include "Aim.h"
#include "Mushroom.h"
#include "../Renderer/Renderer.h"
#include "../Game.h"
#include "../Renderer/Texture.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Components/Physics/AABBColliderComponent.h"
#include "../Components/Physics/RigidBodyComponent.h"

QuestionBlock::QuestionBlock(Game* game, const std::string& texturePath, std::string name)
    : Block(game, texturePath, name)
    , mIsEmpty(false)
    , mEmptyBlockTexture(nullptr)
{
    mCanBounce = true;
    mIsBouncing = false;
    mEmptyBlockTexture = game->GetRenderer()->GetTexture("../Assets/Sprites/Blocks/BlockE.png");
}

void QuestionBlock::StartBounce()
{
    if (mIsEmpty)
        return;

    Block::StartBounce();

    mIsEmpty = true;

    auto animator = GetComponent<AnimatorComponent>();
    if (animator && mEmptyBlockTexture)
    {
        animator->SetTexture(mEmptyBlockTexture);
    }

    Vector2 itemPos = GetPosition() + Vector2(Game::TILE_SIZE*0.1, -Game::TILE_SIZE/2);
    if (name == "Block Mushroom") {
        new Mushroom(GetGame(), GetPosition() + Vector2(0, -Game::TILE_SIZE));
    }else if (name == "Block Coin") {
       // new Aim(GetGame(), itemPos);
    }
    SDL_Log("Bloco ? esvaziado! Item deveria aparecer em (%f, %f)", itemPos.x, itemPos.y);
}

void QuestionBlock::OnUpdate(float deltaTime)
{
    Block::OnUpdate(deltaTime);
}
