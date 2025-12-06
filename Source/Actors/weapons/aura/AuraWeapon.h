// AuraWeapon.h
#pragma once
#include "../../../Components/WeaponComponent.h"  // Inclui a classe base "WeaponComponent"

// Forward declaration para o "corpo" da aura
class AuraActor;

class AuraWeapon : public WeaponComponent {
 public:
  AuraWeapon(class Actor* owner, int updateOrder = 100);

  virtual ~AuraWeapon();

  void OnUpdate(float deltaTime) override;

  void LevelUp() override;

 private:
  class AuraActor* mAura;
};