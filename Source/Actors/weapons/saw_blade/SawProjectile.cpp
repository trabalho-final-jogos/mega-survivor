//
// Created by filipe on 11/4/25.
//

#include "SawProjectile.h"
#include "../../../Game.h"
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Drawing/AnimatorComponent.h" // <-- Usa um Sprite, não um Animator
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../Player.h" // Necessário para pegar o Ator do Player e a Mira
#include "../../Aim.h"  // Necessário para pegar a posição da Mira
#include "../../../Math.h" // Para ToRadians e Vector2::Perpendicular

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
        // --- LÓGICA DE PIERCE ---
        // Aplica dano, mas NÃO chama Kill().
        //other->GetOwner()->ApplyDamage(mDamage);

        // (Para evitar dano infinito no mesmo inimigo, precisaríamos de um
        // sistema de "hit cooldown", mas por enquanto, isto fará ela atravessar)
    }
    // (Ignora Player, outros Projéteis, etc.)
}

void SawProjectile::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
    OnHorizontalCollision(minOverlap, other); // Mesma lógica
}