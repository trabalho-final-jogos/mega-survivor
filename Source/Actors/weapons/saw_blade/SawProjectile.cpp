//
// Created by filipe on 11/4/25.
//

#include "SawProjectile.h"
#include <algorithm>
#include "../../../Game.h"
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Drawing/AnimatorComponent.h" // <-- Usa um Sprite, não um Animator
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../Player.h" // Necessário para pegar o Ator do Player e a Mira
#include "../../Aim.h"  // Necessário para pegar a posição da Mira
#include "../../../Math.h" // Para ToRadians e Vector2::Perpendicular
#include "../../Enemy.h"

SawProjectile::SawProjectile(Game* game, int width, int height)
    : Projectile(game, width, height) // Chama a base (que carrega o spritesheet)
{
    if (mDrawComponent)
    {
        mDrawComponent->AddAnimation("spin_saw", std::vector<int>{14});
    }
}

void SawProjectile::Awake(Actor* owner, const Vector2 &position, float rotation,
                           float lifetime, const Vector2& velocity,
                           float damage, float areaScale)
{
    Projectile::Awake(owner, position, rotation, lifetime, velocity, damage, areaScale);
    if (mDrawComponent)
    {
        mDrawComponent->SetAnimation("spin_saw");
    }

    mEnemiesHit.clear();
    mHitTimer = 0.0f;
}
void SawProjectile::OnUpdate(float deltaTime) {
    // 1. Chama o Update da base (que verifica o LifeTime e chama Kill() se expirar)
    Projectile::OnUpdate(deltaTime);
    if (IsDead()) { return; }

    mHitTimer -= deltaTime;
    if (mHitTimer <= 0.0f)
    {
        mEnemiesHit.clear();
        mHitTimer = mHitCooldown; // Reinicia o timer
    }
}

// Lógica de colisão da Serra (PIERCE)
void SawProjectile::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
    ColliderLayer otherLayer = other->GetLayer();

    if (otherLayer == ColliderLayer::Blocks)
    {
        Kill(); // Quebra em paredes
    }
    else if (otherLayer == ColliderLayer::Enemy)
    {
        Actor* enemyActor = other->GetOwner();
        bool alreadyHit = std::find(mEnemiesHit.begin(), mEnemiesHit.end(), enemyActor) != mEnemiesHit.end();

        if (!alreadyHit) {
            Enemy* enemy = dynamic_cast<Enemy*>(enemyActor);

            if (enemy)
            {
                enemy->TakeDamage(this->GetDamage());
                mEnemiesHit.push_back(enemyActor);
            }
        }
    }
}

void SawProjectile::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
    OnHorizontalCollision(minOverlap, other); // Mesma lógica
}