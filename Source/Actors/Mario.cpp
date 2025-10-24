//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Mario.h"

#include <bits/fs_fwd.h>

#include "../Game.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Components/Physics/RigidBodyComponent.h"
#include "../Components/Physics/AABBColliderComponent.h"
#include "../Components/ParticleSystemComponent.h"
#include "Block.h"
#include <typeinfo>
Mario::Mario(Game* game, const float forwardSpeed, const float jumpSpeed)
        : Actor(game)
        , mIsRunning(false)
        , mIsDead(false)
        , mForwardSpeed(forwardSpeed)
        , mJumpSpeed(jumpSpeed)
        ,mIsBig(false)
        ,mIsInvulnerable(false)
        ,mInvulnerabilityTimer(0.0f)
{
    name = "Mario";
    SetScale(Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
     mDrawComponent = new AnimatorComponent(
        this,                                       // owner
        "../Assets/Sprites/Mario/Mario.png",         // texturePath
        "../Assets/Sprites/Mario/Mario.json",                                         // dataPath (vazio, pois não há animação ainda)
        Game::TILE_SIZE,                            // width do frame (32)
        Game::TILE_SIZE                             // height do frame (32)
    );
    mDrawComponent->SetFlipHorizontal(false);
    mDrawComponent->AddAnimation("idle", std::vector<int>{1});
    mDrawComponent->AddAnimation("run", std::vector<int>{3, 4, 5});
    mDrawComponent->AddAnimation("jump", std::vector<int>{2});
    mDrawComponent->AddAnimation("dead", std::vector<int>{0});
    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);

    mRigidBodyComponent = new RigidBodyComponent(this);

    mColliderComponent = new AABBColliderComponent(this,0,0,Game::TILE_SIZE,Game::TILE_SIZE, ColliderLayer::Player);

}

void Mario::OnProcessInput(const uint8_t* state)
{
    mIsRunning = false;

    float velX = mRigidBodyComponent->GetVelocity().x;

    const float maxForce = 800.0f; // ajustável

    Vector2 force = Vector2::Zero;

    if(state[SDL_SCANCODE_D])
    {
        mIsRunning = true;
        if(mDrawComponent) { mDrawComponent->SetFlipHorizontal(false); }

        if(velX < mForwardSpeed)
        {
            force.x = maxForce;
        }
    }
    else if(state[SDL_SCANCODE_A])
    {
        mIsRunning = true;
        if(mDrawComponent) { mDrawComponent->SetFlipHorizontal(true); }

        if(velX > -mForwardSpeed)
        {
            force.x = -maxForce;
        }
    }

    mRigidBodyComponent->ApplyForce(force);

    if(state[SDL_SCANCODE_SPACE] && mIsOnGround)
    {
        Vector2 vel = mRigidBodyComponent->GetVelocity();
        vel.y = mJumpSpeed;
        SetOffGround();
        mRigidBodyComponent->SetVelocity(vel);
    }
}
void Mario::OnUpdate(float deltaTime)
{
    if (mIsInvulnerable)
    {
        // Decrementa o timer
        mInvulnerabilityTimer -= deltaTime;

        // Efeito de piscar (alterna visibilidade)
        if (mDrawComponent)
        {
            // Pisca a cada ~0.1 segundos (ajuste conforme necessário)
            bool show = static_cast<int>(mInvulnerabilityTimer * 10.0f) % 2 == 0;
            mDrawComponent->SetVisible(show); // Assumindo que AnimatorComponent tem SetVisible
        }

        // Se o tempo acabou...
        if (mInvulnerabilityTimer <= 0.0f)
        {
            mIsInvulnerable = false;
            // Garante que o sprite fique visível no final
            if (mDrawComponent)
            {
                mDrawComponent->SetVisible(true);
            }
            SDL_Log("Mario nao esta mais invulneravel.");
        }
    }
    if (mRigidBodyComponent)
    {
        Vector2 velocity = mRigidBodyComponent->GetVelocity();

        if (!Math::NearlyZero(velocity.y))
        {
            SetOffGround(); // Ou mIsOnGround = false;
        }
    }

    if (GetPosition().y > (Game::WINDOW_HEIGHT + Game::TILE_SIZE)) // Adiciona um buffer
    {
        Kill();
    }
    ManageAnimations();
}

void Mario::ManageAnimations()
{
    if (!mDrawComponent) return;

    if (mIsDead)
    {
        mDrawComponent->SetAnimation("dead");
        return;
    }

    const Vector2 vel = mRigidBodyComponent->GetVelocity();

    if (!mIsOnGround)
    {
        mDrawComponent->SetAnimation("jump");
    }
    else if (mIsRunning && std::abs(vel.x) > 10.0f)
    {
        mDrawComponent->SetAnimation("run");
    }
    else
    {
        mDrawComponent->SetAnimation("idle");
    }
}

void Mario::Kill()
{
    mDrawComponent->SetAnimation("dead");
    mIsDead = true;
    mRigidBodyComponent->SetVelocity(Vector2::Zero);
    mRigidBodyComponent->SetEnabled(false);
    mColliderComponent->SetEnabled(false);
}

void Mario::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
    ColliderLayer otherLayer = other->GetLayer();
    if (otherLayer == ColliderLayer::Enemy)
    {
        if (!mIsInvulnerable) {
            if (!mIsBig) {
                Kill();
            }else {
                Shrink();
            }
        }
    }
    if (otherLayer == ColliderLayer::PowerUp) {
        SDL_Log("Mario colidiu horizontalmente com PowerUp!");
        Actor* powerUp = other->GetOwner();
        powerUp->SetState(ActorState::Destroy);
        Grow();
    }
}

void Mario::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{

    ColliderLayer otherLayer = other->GetLayer();

    if (otherLayer == ColliderLayer::Enemy)
    {
        if (minOverlap < 0.0f)
        {
            Actor* enemy = other->GetOwner();
            if (enemy)
            {
                enemy->Kill();
            }

            if (mRigidBodyComponent)
            {
                Vector2 vel = mRigidBodyComponent->GetVelocity();
                vel.y = mJumpSpeed;
                mRigidBodyComponent->SetVelocity(vel);
                SetOffGround();
            }
        }
        else
        {
            SDL_Log("Mario atingido verticalmente por inimigo! Chamando Kill(). Overlap: %f", minOverlap);
            if (!mIsInvulnerable) {
                if (!mIsBig) {
                    Kill();

                }else {
                    Shrink();
                }
            }
        }
    }else if (otherLayer == ColliderLayer::Blocks)
    {
        // Verifica se a colisão foi DE BAIXO PARA CIMA
        if (minOverlap > 0.0f)
        {
            // Pega o Ator do colisor (o Bloco)
            Actor* blockActor = other->GetOwner();

            Block* block = dynamic_cast<Block*>(blockActor);
            // Se a conversão funcionou (é realmente um Bloco)...
            if (block)
            {
                block->StartBounce();
            }

            // Opcional: Zera a velocidade Y do Mario para ele "bater a cabeça"
            if(mRigidBodyComponent)
            {
                Vector2 vel = mRigidBodyComponent->GetVelocity();
                if (vel.y < 0.0f)
                {
                    vel.y = 0.0f;
                    mRigidBodyComponent->SetVelocity(vel);
                }
            }
        }
    }else  if (otherLayer == ColliderLayer::PowerUp) {
        SDL_Log("Mario colidiu vertical com PowerUp!");
        Actor* powerUp = other->GetOwner();
        powerUp->SetState(ActorState::Destroy);
        Grow();
    }
}
void Mario::Grow()
{
    if (mIsBig) { return; }

    mIsBig = true;

    const std::string bigTexturePath = "../Assets/Sprites/SuperMario/SuperMario.png";
    const std::string bigJsonPath = "../Assets/Sprites/SuperMario/SuperMario.json";

    if (mDrawComponent->LoadSheet(bigTexturePath, bigJsonPath)) {
        // Adiciona animações
        mDrawComponent->AddAnimation("idle", {0});
        mDrawComponent->AddAnimation("run", {6,7,8});
        mDrawComponent->AddAnimation("jump", {2});
        mDrawComponent->SetAnimation("idle");
        mDrawComponent->SetAnimFPS(10.0f);
    }
    SDL_Log("Mario cresceu! Agora ele é grande.");

    Vector2 currentScale = GetScale();
    SetScale(Vector2(currentScale.x, currentScale.y * 2.0f));
    Vector2 currentPos = GetPosition();
    SetPosition(currentPos + Vector2(0.0f, -Game::TILE_SIZE / 2.0f));
    mColliderComponent->SetSize(Vector2(Game::TILE_SIZE,Game::TILE_SIZE * 2));
}

void Mario::Shrink() {
    if (!mIsBig) { return; }

    mIsBig = false;
    mIsInvulnerable = true;
    mInvulnerabilityTimer = INVULNERABILITY_DURATION;
    const std::string bigTexturePath = "../Assets/Sprites/Mario/Mario.png";
    const std::string bigJsonPath = "../Assets/Sprites/Mario/Mario.json";

    if (mDrawComponent->LoadSheet(bigTexturePath, bigJsonPath)) {
        mDrawComponent->AddAnimation("idle", std::vector<int>{1});
        mDrawComponent->AddAnimation("run", std::vector<int>{3, 4, 5});
        mDrawComponent->AddAnimation("jump", std::vector<int>{2});
        mDrawComponent->AddAnimation("dead", std::vector<int>{0});
        mDrawComponent->SetAnimation("idle");
        mDrawComponent->SetAnimFPS(10.0f);

    }
    SDL_Log("Mario agora é pequeno");

    Vector2 currentScale = GetScale();
    SetScale(Vector2(currentScale.x, currentScale.y/2));
    Vector2 currentPos = GetPosition();
    SetPosition(currentPos + Vector2(0.0f, Game::TILE_SIZE / 2.0f));
    mColliderComponent->SetSize(Vector2(Game::TILE_SIZE,Game::TILE_SIZE));

}