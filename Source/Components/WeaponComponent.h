// WeaponComponent.h
#pragma once
#include "Component.h"
#include "../Math.h"

class WeaponComponent : public Component
{
public:
    WeaponComponent(class Actor* owner, int updateOrder = 100);

    // Loop de atualização (chama OnUpdate)
    void Update(float deltaTime) override;

    // Método virtual puro: toda arma DEVE implementar sua própria lógica de update
    virtual void OnUpdate(float deltaTime) = 0;

};