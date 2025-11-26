//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Goomba.h"

#include "Player.h"
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
                0, 0,
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
        const Player* player = GetGame()->GetPlayer();

        if (player)
        {
                Vector2 playerPos = player->GetPosition();
                Vector2 myPos = GetPosition();

                // 2. Calcula o vetor direção
                Vector2 direction = playerPos - myPos;

                // 3. Normaliza para ter apenas a direção (comprimento 1)
                if (direction.LengthSq() > 0.0f)
                {
                        direction.Normalize();
                }

                // 4. Aplica o movimento (usando a velocidade e o fator de slow)
                // (Lembrando de não usar gravidade no eixo Y em top-down)
                if (mRigidBodyComponent)
                {
                        // Se você tem lógica de slow (mSpeedFactor), use-a aqui
                        // float speed = mWalkSpeed * mSpeedFactor;
                        float speed = mForwardSpeed; // Simples por enquanto

                        mRigidBodyComponent->SetVelocity(direction * speed);
                }
        }
}

void Goomba::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
        if (other->GetLayer() == ColliderLayer::Player) {
                Actor* player = other->GetOwner();
                Player* mario = dynamic_cast<Player*>(player);
                if (!mario->isInvulnerable()) {
                        if (mario->isBig()) {
                                mario->Shrink();
                        }else {
                                mario->Kill();
                        }
                }
        }

        if (other->GetLayer() == ColliderLayer::Blocks || other->GetLayer() == ColliderLayer::Enemy) {
                Vector2 currentVelocity = mRigidBodyComponent->GetVelocity();

                if (minOverlap < 0) {
                        mRigidBodyComponent->SetVelocity(Vector2(-mForwardSpeed, currentVelocity.y));
                } else {
                        mRigidBodyComponent->SetVelocity(Vector2(mForwardSpeed, currentVelocity.y));
                }
        }
}

void Goomba::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
        ColliderLayer otherLayer = other->GetLayer();
        if (otherLayer == ColliderLayer::Player) {
                Actor* player = other->GetOwner();

                Player* mario = dynamic_cast<Player*>(player);
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
