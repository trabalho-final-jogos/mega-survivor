#pragma once
#include "../Projectile.h"

class Bullet : public Projectile
{
public:
    Bullet(class Game* game, int width, int height);

    // Sobrescreve (overrides) os métodos de colisão da base
    // para adicionar a lógica de dano a inimigos
    void OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision(const float minOverlap, AABBColliderComponent* other) override;
};