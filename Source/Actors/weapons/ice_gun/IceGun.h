// IceGun.h
#pragma once
#include "../../../Components/WeaponComponent.h"
#include "../../../Components/ProjectilePoolComponent.h"
#include "../../Aim.h" // Inclui a classe base
#include "../../../Math.h"
#include <vector>

// Forward declarations
class ProjectilePoolComponent;
class Aim;

class IceGun : public WeaponComponent
{
public:
    IceGun(class Actor* owner,int updateOrder = 100);
    ~IceGun();

    // Implementa a lógica de cooldown e disparo automático
    void OnUpdate(float deltaTime) override;
    void LevelUp() override;
private:
    void FireShot(); // Dispara o leque

    ProjectilePoolComponent* mProjectilePool;
    Aim* mAim;

    // --- Constantes da IceGun ---
    const float COOLDOWN_TIME = 1.5f;       // Tempo (seg) entre os disparos
    const int NUM_PROJECTILES = 5;          // 5 projéteis por disparo
    const float SPREAD_ANGLE = 30.0f;     // 30 graus de leque total
    const float PROJECTILE_SPEED = 500.0f;
    const float PROJECTILE_LIFETIME = 0.5f; // Curto alcance
    const int PARTICLE_WIDTH = 12;          // Tamanho do sprite de gelo
    const int PARTICLE_HEIGHT = 12;
    const int POOL_SIZE = 60;               // (5 projéteis * 6 rajadas)

    float mCooldownTimer;
};