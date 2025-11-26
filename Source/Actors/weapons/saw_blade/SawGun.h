#pragma once
#include "../../../Components/WeaponComponent.h"
#include "../../../Components/ProjectilePoolComponent.h"
#include "../../Aim.h" // Inclui a classe base
#include "../../../Math.h"
#include <vector>
#include <random>

class SawGun : public WeaponComponent
{
public:
    SawGun(class Actor* owner, int updateOrder = 100);
    ~SawGun();

    void OnUpdate(float deltaTime) override;
    void LevelUp() override;

private:
    void FireShot();

    ProjectilePoolComponent* mProjectilePool;

    // Lista das 4 direções diagonais (normalizadas)
    std::vector<Vector2> mDiagonalVectors;

    // Gerador de números aleatórios (para Nível 1)
    std::mt19937 mRNG; // Mersenne Twister
    std::uniform_int_distribution<int> mRandDiag; // Distribuição (0-3)

    // --- Estatísticas (Variáveis) ---
    float mCooldownTime;
    int mNumProjectiles;
    float mProjectileSpeed;
    float mProjectileLifetime;
    float mDamage;
    float mAreaScale;

    // --- Timer ---
    float mCooldownTimer;
};