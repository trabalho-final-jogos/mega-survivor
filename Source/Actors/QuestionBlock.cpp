#include "QuestionBlock.h"

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
    // Só faz algo se o bloco ainda não foi ativado
    if (mIsEmpty)
        return;

    // Faz o "pulo" do bloco (subir e descer)
    Block::StartBounce();

    // Marca como vazio
    mIsEmpty = true;

    // Troca a textura para o bloco vazio
    auto animator = GetComponent<AnimatorComponent>();
    if (animator && mEmptyBlockTexture)
    {
        animator->SetTexture(mEmptyBlockTexture);
    }

    // Solta o item (exemplo)
    Vector2 itemPos = GetPosition() + Vector2(Game::TILE_SIZE*0.1, -Game::TILE_SIZE/2);
    if (name == "Block Mushroom")
        new Mushroom(GetGame(), itemPos);
    SDL_Log("Bloco ? esvaziado! Item deveria aparecer em (%f, %f)", itemPos.x, itemPos.y);
}

void QuestionBlock::OnUpdate(float deltaTime)
{
    // Mantém o comportamento normal de bounce do Block
    Block::OnUpdate(deltaTime);
}
