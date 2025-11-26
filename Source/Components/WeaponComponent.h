// WeaponComponent.h
#pragma once
#include "Component.h"
#include "../Math.h"
#include "../Actors/weapons/WeaponType.h"


class WeaponComponent : public Component
{
public:
    WeaponComponent(class Actor* owner,WeaponType type, int updateOrder = 100);

    // Loop de atualização (chama OnUpdate)
    void Update(float deltaTime) override;

    virtual void OnUpdate(float deltaTime) = 0;
    virtual void LevelUp() = 0;

    // Getter para o tipo de arma
    WeaponType GetType() const { return mType; }
    int GetLevel() const { return mLevel; }

protected:
    WeaponType mType;
    int mLevel;
};
