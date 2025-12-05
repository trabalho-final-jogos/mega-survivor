// LaserGun.h
#pragma once
#include "../../../Components/WeaponComponent.h"
#include "../../../Math.h"
#include <vector>

class ProjectilePoolComponent;
class Aim;

class LaserGun : public WeaponComponent
{
public:
    LaserGun(class Actor* owner, int updateOrder = 100);
    ~LaserGun();

    void OnUpdate(float deltaTime) override;
    void LevelUp() override;

private:
    void FireShot();

    ProjectilePoolComponent* mProjectilePool;
    Aim* mAim;

    float mCooldownTime;
    float mProjectileSpeed;
    float mProjectileLifetime;
    float mDamage;
    float mAreaScale;
    int mNumBounces;

    float mCooldownTimer;
};