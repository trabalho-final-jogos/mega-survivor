#include "WeaponComponent.h"
#include "../Actors/Actor.h"

WeaponComponent::WeaponComponent(class Actor* owner,
                                 WeaponType type,
                                 int updateOrder)
    : Component(owner, updateOrder),
      mType(type),
      mLevel(0)  // Todas as armas começam no nível 0 (ainda não equipadas)
{}

void WeaponComponent::Update(float deltaTime) {
  // Chama a lógica de cooldown/disparo da arma filha
  OnUpdate(deltaTime);

  // Chama o Update da base (se houver lógica futura)
  Component::Update(deltaTime);
}