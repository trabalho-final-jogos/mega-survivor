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

private:
    void FireShot(); // Dispara um único tiro
    ProjectilePoolComponent* mProjectilePool;

    // --- Componentes Cacheados ---
    class Aim* mAimer;

    // --- Constantes da Arma ---
    const float COOLDOWN_TIME = 0.8f;
    const int BURST_COUNT = 3;
    const float BURST_DELAY = 0.04f;
    const float PROJECTILE_SPEED = 450.0f;
    const float PROJECTILE_LIFETIME = 1.0f;
    const int PARTICLE_WIDTH = 16;
    const int PARTICLE_HEIGHT = 16;
    const int POOL_SIZE = 50;

    // --- Estado dos Timers ---
    float mCooldownTimer;
    int mBurstShotsLeft;
    float mBurstTimer;
    bool mIsFiringBurst;
    Vector2 mInheritedVel;
    Vector2 mBurstVelocity;
    Vector2 mBurstDirection;
};