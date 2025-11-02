#include "WeaponComponent.h"
#include "../Actors/Actor.h"

WeaponComponent::WeaponComponent(class Actor* owner, int updateOrder)
    : Component(owner, updateOrder)
{
    // O construtor base (Component) já registra este componente no Actor
}

void WeaponComponent::Update(float deltaTime)
{
    // Chama a lógica de cooldown/disparo da arma filha
    OnUpdate(deltaTime);
    
    // Chama o Update da base (se houver lógica futura)
    Component::Update(deltaTime);
}