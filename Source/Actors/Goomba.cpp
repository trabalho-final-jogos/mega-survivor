//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Goomba.h"

#include "Mario.h"
#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Components/Physics/RigidBodyComponent.h"
#include "../Components/Physics/AABBColliderComponent.h"

Goomba::Goomba(Game* game, float forwardSpeed, float deathTime)
        : Actor(game)
        , mDyingTimer(deathTime)
        , mIsDying(false)
        , mForwardSpeed(forwardSpeed)
{

        mDrawComponent = new AnimatorComponent(
               this,
               "../Assets/Sprites/Goomba/Goomba.png",
               "../Assets/Sprites/Goomba/Goomba.json",
               Game::TILE_SIZE,
               Game::TILE_SIZE
       );
        mDrawComponent->SetEnabled(true);
        mDrawComponent->SetVisible(true);
        mDrawComponent->AddAnimation("walk", std::vector<int>{1,2});
        mDrawComponent->AddAnimation("dead", std::vector<int>{0});
        mDrawComponent->SetAnimation("walk");
        mDrawComponent->SetAnimFPS(6.0f);


        name = "Goomba";

        mRigidBodyComponent = new RigidBodyComponent(this);
        mRigidBodyComponent->SetVelocity(Vector2(-forwardSpeed, 0.0f));
        mColliderComponent = new class AABBColliderComponent(
                this,
                0, 0, // dx, dy
                Game::TILE_SIZE, Game::TILE_SIZE,
                ColliderLayer::Enemy,
                false
        );




}

void Goomba::Kill()
{
        mIsDying = true;
        mDrawComponent->SetAnimation("dead");
        mRigidBodyComponent->SetEnabled(false);
        mColliderComponent->SetEnabled(false);

}

void Goomba::OnUpdate(float deltaTime)
{
        if (mIsDying)
        {
                mDyingTimer -= deltaTime;
                if (mDyingTimer <= 0)
                {
                        mState = ActorState::Destroy;

                }
        }
        float bottomOfLevel = Game::LEVEL_HEIGHT * Game::TILE_SIZE;

        if (GetPosition().y > (bottomOfLevel + (Game::TILE_SIZE * 2)))
        {
                SetState(ActorState::Destroy);
        }
}

void Goomba::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
        if (other->GetLayer() == ColliderLayer::Player) {
                Actor* player = other->GetOwner();
                Mario* mario = dynamic_cast<Mario*>(player);
                if (!mario->isInvulnerable()) {
                        if (mario->isBig()) {
                                mario->Shrink();
                        }else {
                                mario->Kill();
                        }
                }
        }

        // Se colidiu com blocos (Layer::Block) ou outros inimigos (Layer::Enemy)
        if (other->GetLayer() == ColliderLayer::Blocks || other->GetLayer() == ColliderLayer::Enemy) {
                // Inverte a direção baseada no sinal da sobreposição
                Vector2 currentVelocity = mRigidBodyComponent->GetVelocity();

                if (minOverlap < 0) {
                        // Colisão pela direita - deve ir para a esquerda
                        mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, currentVelocity.y));
                } else {
                        // Colisão pela esquerda - deve ir para a direita
                        mRigidBodyComponent->SetVelocity(Vector2(mForwardSpeed, currentVelocity.y));
                }
        }
}

void Goomba::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
        ColliderLayer otherLayer = other->GetLayer();
        if (otherLayer == ColliderLayer::Player) {
                Actor* player = other->GetOwner();

                Mario* mario = dynamic_cast<Mario*>(player);
                if (!mario->isInvulnerable()) {
                        if (minOverlap<0) {
                                if (mario->isBig()) {
                                        mario->Shrink();
                                }else {
                                        mario->Kill();
                                }
                        }
                }
                SDL_Log("Goomba::OnVerticalCollision %f", minOverlap );

        }
}
