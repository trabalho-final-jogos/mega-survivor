// LaserProjectile.h
#pragma once
#include "../Projectile.h" // A classe base (antiga Particle)

class LaserProjectile : public Projectile
{
public:
    LaserProjectile(class Game* game, int width, int height);

    void Awake(Actor* owner, const Vector2 &position, float rotation,
               float lifetime, const Vector2& velocity,
               float damage, float areaScale) override;

    // O Update é crucial para a lógica de ricochete
    void OnUpdate(float deltaTime) override;

    // A colisão com objetos (pierce)
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;

    // Método específico para esta classe
    void SetBounceCount(int bounces) { mBouncesLeft = bounces; }

private:
    int mBouncesLeft; // Quantos ricochetes ainda tem
};