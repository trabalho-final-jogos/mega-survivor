// IceProjectile.h
#pragma once
#include "../Projectile.h" // Inclui a classe base

class IceProjectile : public Projectile
{
public:
    IceProjectile(class Game* game, int width, int height);

    // Sobrescreve Awake para tocar a animação de gelo
    void Awake(Actor *owner, const Vector2 &position, float rotation,
               float lifetime, const Vector2 &velocity, float damage, float areaScale) override;

    // Sobrescreve as colisões para aplicar o efeito "slow"
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;

private:
    // Constantes do Efeito
    const float SLOW_FACTOR = 0.5f; // 50% da velocidade
    const float SLOW_DURATION = 3.0f; // 3 segundos
};