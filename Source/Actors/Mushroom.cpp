// Mushroom.cpp
#include "Mushroom.h"
#include "Mario.h"
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
        0, 0, // dx, dy
        Game::TILE_SIZE, Game::TILE_SIZE,
        ColliderLayer::PowerUp,
        false // isStatic
    );
}

void Mushroom::OnUpdate(float deltaTime)
{
    Actor::OnUpdate(deltaTime);

    // --- Pop-up: mover até o targetY diretamente ---
    if (mIsPoppingUp)
    {
        // calcula nova posição Y
        float moveAmount = POP_UP_SPEED * deltaTime;
        Vector2 pos = GetPosition();
        float newY = pos.y - moveAmount; // sobe (y menor)

        // se passou do alvo, faz snap no target e finaliza o pop-up
        if (newY <= mPopTargetY)
        {
            newY = mPopTargetY;
            SetPosition(Vector2(pos.x, newY));

            // terminou de subir: habilita gravidade e começa a andar
            mIsPoppingUp = false;

            // zera velocidade vertical e habilita física
            if (mRigidBody)
            {
                mRigidBody->SetVelocity(Vector2(-mWalkSpeed, 0.0f)); // começa a andar (x)
                mRigidBody->SetApplyGravity(true); // agora cai/é afetado por gravidade
            }
        }
        else
        {
            // ainda subindo
            SetPosition(Vector2(pos.x, newY));
        }

        // enquanto estiver subindo, não fazer mais nada
        return;
    }

    // --- Lógica normal após sair do bloco ---
    float bottomOfLevel = Game::LEVEL_HEIGHT * Game::TILE_SIZE;
    if (GetPosition().y > (bottomOfLevel + (Game::TILE_SIZE * 2)))
    {
        SetState(ActorState::Destroy);
    }
}

void Mushroom::OnHorizontalCollision(float minOverlap, AABBColliderComponent* other)
{
    // Se ainda está saindo do bloco, ignora colisões horizontais
    if (mIsPoppingUp) return;

    ColliderLayer otherLayer = other->GetLayer();

    // Se colidir com Blocos, inverte a direção (similar ao Goomba)
    if (otherLayer == ColliderLayer::Blocks)
    {
        if (mRigidBody)
        {
            Vector2 vel = mRigidBody->GetVelocity();

            if (minOverlap > 0.0f) { vel.x = mWalkSpeed; } // Bateu na esquerda, vai pra direita
            else if (minOverlap < 0.0f) { vel.x = -mWalkSpeed; } // Bateu na direita, vai pra esquerda

            mRigidBody->SetVelocity(vel);
        }
    }
    if (auto mario = dynamic_cast<Mario*>(other->GetOwner()))
    {
        mario->Grow(); // 🍄 faz o Mario crescer
        SetState(ActorState::Destroy); // remove o cogumelo
        SDL_Log("Mario pegou o cogumelo H!");
        return;
    }
}

void Mushroom::OnVerticalCollision(float minOverlap, AABBColliderComponent *other) {
    if (auto mario = dynamic_cast<Mario*>(other->GetOwner()))
    {
        mario->Grow(); // 🍄 faz o Mario crescer
        SetState(ActorState::Destroy); // remove o cogumelo
        SDL_Log(" Mario pegou o cogumelo! V");
        return;
    }
}
