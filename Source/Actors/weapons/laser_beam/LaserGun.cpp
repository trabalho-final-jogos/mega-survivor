//
// Created by filipe on 11/6/25.
//

#include "LaserGun.h"
#include "LaserProjectile.h"
#include "../../../Components/ProjectilePoolComponent.h"
#include "../../../Game.h"
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Drawing/AnimatorComponent.h" // <-- Usa um Sprite, não um Animator
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../Player.h" // Necessário para pegar o Ator do Player e a Mira
#include "../../Aim.h"  // Necessário para pegar a posição da Mira
#include "../../../Math.h" // Para ToRadians e Vector2::Perpendicular


LaserGun::LaserGun(Actor* owner, int updateOrder)
    : WeaponComponent(owner, WeaponType::LaserGun, updateOrder)
    , mProjectilePool(nullptr)
    , mAim(nullptr)
    , mCooldownTimer(0.0f)
{
    // Cria o pool
    mProjectilePool = new ProjectilePoolComponent();

    // Preenche o pool com o PROJÉTIL DE LASER
    for (int i = 0; i < 20; i++) // Pool de 20 (Lasers duram mais)
    {
        auto laser = new LaserProjectile(mOwner->GetGame(), 16, 16); // Tamanho
        mProjectilePool->AddObjectToPool(laser);
    }

    Player* player = static_cast<Player*>(mOwner);
    mAim = player->GetAim();

    LevelUp(); // Define Nível 1
}

LaserGun::~LaserGun()
{
    delete mProjectilePool;
}

void LaserGun::LevelUp()
{
    mLevel++;
    SDL_Log("LaserGun subiu para o Nível %d!", mLevel);

    switch(mLevel)
    {
        case 1:
            mCooldownTime = 2.0f;
            mProjectileSpeed = 350.0f;
            mProjectileLifetime = 5.0f; // Dura 5 segundos
            mDamage = 8.0f;
            mAreaScale = 1.0f;
            mNumBounces = 2; // 2 ricochetes
            break;
        case 2:
            mCooldownTime = 1.8f;
            mDamage = 12.0f;
            mNumBounces = 3; // 3 ricochetes
            break;
        case 3:
            mProjectileLifetime = 7.0f; // Dura mais
            mDamage = 15.0f;
            mNumBounces = 4; // 4 ricochetes
            break;
        default:
            mNumBounces++; // Mais um ricochete por nível
            mDamage += 5.0f;
            break;
    }
}

void LaserGun::OnUpdate(float deltaTime)
{
    if (mCooldownTimer > 0.0f) { mCooldownTimer -= deltaTime; }

    if (mCooldownTimer <= 0.0f)
    {
        FireShot();
        mCooldownTimer = mCooldownTime;
    }
}

void LaserGun::FireShot()
{
    if (!mAim) { return; }

    Player* player = static_cast<Player*>(mOwner);
    Vector2 playerPos = player->GetPosition();
    Vector2 aimerPos = mAim->GetPosition();
    Vector2 direction = (aimerPos - playerPos);
    direction.Normalize();
    Vector2 playerVelocity = player->GetComponent<RigidBodyComponent>()->GetVelocity();

    // (O Laser pode ignorar a velocidade do jogador para ser mais previsível)
    // Vector2 finalVelocity = (direction * mProjectileSpeed) + playerVelocity;
    Vector2 finalVelocity = (direction * mProjectileSpeed);


    Projectile* p = mProjectilePool->GetDeadObject();
    if (!p) { return; } // Pool vazio

    // Tenta fazer o cast para o tipo específico
    LaserProjectile* laser = dynamic_cast<LaserProjectile*>(p);
    if (laser)
    {
        // 1. Define as estatísticas de ricochete
        laser->SetBounceCount(mNumBounces);

        // 2. "Acorda" o projétil
        laser->Awake(mOwner, playerPos, mOwner->GetRotation(), mProjectileLifetime,
                     finalVelocity, mDamage, mAreaScale); // (Valores padrão de slow)
    }
}