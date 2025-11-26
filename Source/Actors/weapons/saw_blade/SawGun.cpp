//
// Created by filipe on 11/4/25.
//

#include "SawGun.h"
#include "SawProjectile.h"
#include "../../../Components/ProjectilePoolComponent.h"
#include "../../../Game.h"
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Drawing/AnimatorComponent.h" // <-- Usa um Sprite, não um Animator
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../Player.h" // Necessário para pegar o Ator do Player e a Mira
#include "../../Aim.h"  // Necessário para pegar a posição da Mira
#include "../../../Math.h" // Para ToRadians e Vector2::Perpendicular

SawGun::SawGun(Actor* owner, int updateOrder)
    : WeaponComponent(owner, WeaponType::SawGun, updateOrder)
    , mProjectilePool(nullptr)
    , mCooldownTimer(0.0f)
    , mRNG(std::random_device{}()) // Inicializa o gerador aleatório
    , mRandDiag(0, 3)             // Gera números entre 0 e 3
{
    // 1. Cria o pool
    mProjectilePool = new ProjectilePoolComponent();

    // 2. Preenche o pool com o PROJÉTIL DE SERRA
    for (int i = 0; i < 50; i++) // Pool de 50 serras
    {
        auto saw = new SawProjectile(mOwner->GetGame(), 24, 24); // Tamanho da serra
        mProjectilePool->AddObjectToPool(saw);
    }

    // 3. Define as 4 direções diagonais (normalizadas)
    Vector2 dirNE( 1.0f, -1.0f);
    dirNE.Normalize(); // Modifica o dirNE no lugar
    mDiagonalVectors.push_back(dirNE); // Adiciona o vetor já normalizado

    Vector2 dirSE( 1.0f,  1.0f);
    dirSE.Normalize();
    mDiagonalVectors.push_back(dirSE);

    Vector2 dirSW(-1.0f,  1.0f);
    dirSW.Normalize();
    mDiagonalVectors.push_back(dirSW);

    Vector2 dirNW(-1.0f, -1.0f);
    dirNW.Normalize();
    mDiagonalVectors.push_back(dirNW); // Noroeste (NW)

    // 4. Define as estatísticas do Nível 1
    LevelUp();
}

SawGun::~SawGun()
{
    delete mProjectilePool;
}

void SawGun::LevelUp()
{
    mLevel++;
    SDL_Log("SawGun subiu para o Nível %d!", mLevel);

    switch(mLevel)
    {
        case 1:
            mNumProjectiles = 1; // 1 projétil
            mCooldownTime = 2.0f;
            mProjectileSpeed = 250.0f;
            mProjectileLifetime = 2.0f;
            mDamage = 12.0f;
            mAreaScale = 1.0f;
            break;
        case 2:
            mNumProjectiles = 2; // 2 projéteis
            mCooldownTime = 1.8f;
            mDamage = 15.0f;
            break;
        case 3:
            mNumProjectiles = 3; // 3 projéteis
            mCooldownTime = 1.6f;
            mAreaScale = 1.2f; // Serras maiores
            break;
        case 4:
            mNumProjectiles = 4; // 4 projéteis (completo)
            mCooldownTime = 1.4f;
            mDamage = 20.0f;
            break;
        default:
            // A cada nível extra, diminui o cooldown
            mCooldownTime = std::max(0.5f, mCooldownTime - 0.1f);
            mDamage += 5.0f;
            break;
    }
}

void SawGun::OnUpdate(float deltaTime)
{
    if (mCooldownTimer > 0.0f) { mCooldownTimer -= deltaTime; }

    if (mCooldownTimer <= 0.0f)
    {
        FireShot();
        mCooldownTimer = mCooldownTime;
    }
}

void SawGun::FireShot()
{
    Player* player = static_cast<Player*>(mOwner);
    Vector2 playerPos = player->GetPosition();
    Vector2 playerVelocity = player->GetComponent<RigidBodyComponent>()->GetVelocity();

    // Lógica de disparo baseada no nível

    if (mNumProjectiles == 1) // Nível 1: 1 diagonal aleatória
    {
        int randIndex = mRandDiag(mRNG); // Pega um índice aleatório (0-3)
        Vector2 dir = mDiagonalVectors[randIndex];
        Vector2 finalVelocity = (dir * mProjectileSpeed) + playerVelocity;

        Projectile* p = mProjectilePool->GetDeadObject();
        if (p)
        {
            p->Awake(mOwner, playerPos, 0.0f, mProjectileLifetime,
                     finalVelocity, mDamage, mAreaScale);
        }
    }
    else if (mNumProjectiles == 2) // Nível 2: 2 diagonais opostas
    {
        // (Poderia ser aleatório, mas vamos fixar em NE e SW)
        Vector2 dir1 = mDiagonalVectors[0]; // NE
        Vector2 dir2 = mDiagonalVectors[2]; // SW

        Projectile* p1 = mProjectilePool->GetDeadObject();
        if (p1)
        {
            p1->Awake(mOwner, playerPos, 0.0f, mProjectileLifetime,
                      (dir1 * mProjectileSpeed) + playerVelocity, mDamage, mAreaScale);
        }
        Projectile* p2 = mProjectilePool->GetDeadObject();
        if (p2)
        {
            p2->Awake(mOwner, playerPos, 0.0f, mProjectileLifetime,
                      (dir2 * mProjectileSpeed) + playerVelocity, mDamage, mAreaScale);
        }
    }
    else if (mNumProjectiles == 3) // Nível 3: 3 diagonais
    {
        // (Vamos pular uma, ex: SE)
        Vector2 dirs[3] = { mDiagonalVectors[0], mDiagonalVectors[2], mDiagonalVectors[3] };
        for (const auto& dir : dirs)
        {
            Projectile* p = mProjectilePool->GetDeadObject();
            if (!p) break;
            p->Awake(mOwner, playerPos, 0.0f, mProjectileLifetime,
                     (dir * mProjectileSpeed) + playerVelocity, mDamage, mAreaScale);
        }
    }
    else // Nível 4 ou mais: Todas as 4 diagonais
    {
        for (const auto& dir : mDiagonalVectors)
        {
            Projectile* p = mProjectilePool->GetDeadObject();
            if (!p) break;
            p->Awake(mOwner, playerPos, 0.0f, mProjectileLifetime,
                     (dir * mProjectileSpeed) + playerVelocity, mDamage, mAreaScale);
        }
    }
}