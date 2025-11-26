#pragma once
#include "../../../Components/WeaponComponent.h"
#include "../../Aim.h"
#include "../../../Components/ProjectilePoolComponent.h"


class MainGun : public WeaponComponent
{
public:
    MainGun(class Actor* owner, int updateOrder = 100);
    ~MainGun();

    // Implementa a lógica de cooldown e disparo automático
    void OnUpdate(float deltaTime) override;

    void LevelUp() override;

private:
    void FireShot(); // Dispara um único tiro
    ProjectilePoolComponent* mProjectilePool;

    // --- Componentes Cacheados ---
    class Aim* mAimer;

    // --- Constantes da Arma ---
    const int PARTICLE_WIDTH = 16;
    const int PARTICLE_HEIGHT = 16;
    const int POOL_SIZE = 50;

    float mCooldownTime;
    int mBurstCount;
    float mBurstDelay;
    float mProjectileSpeed;
    float mProjectileLifetime;

    float mDamage;
    float mAreaScale;
    // --- Estado dos Timers ---
    float mCooldownTimer;
    int mBurstShotsLeft;
    float mBurstTimer;
    bool mIsFiringBurst;

    Vector2 mInheritedVel;
    Vector2 mBurstVelocity;
    Vector2 mBurstDirection;
};