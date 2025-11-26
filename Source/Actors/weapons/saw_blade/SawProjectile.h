#pragma once
#include "../Projectile.h" // Inclui a classe base


class SawProjectile : public Projectile
{
public:
    SawProjectile(class Game* game, int width, int height);

    void Awake(Actor* owner, const Vector2 &position, float rotation,
               float lifetime, const Vector2& velocity,
               float damage, float areaScale) override;

    // A serra tem colisão especial: atravessa inimigos
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
};
