// AuraWeapon.h
#pragma once
#include "../../../Components/WeaponComponent.h"  // Inclui a classe base "WeaponComponent"

// Forward declaration para o "corpo" da aura
class AuraActor;

class AuraWeapon : public WeaponComponent {
 public:
  /**
   * @param owner O Ator (Player) que possui esta arma
   */
  AuraWeapon(class Actor* owner, int updateOrder = 100);

  /**
   * Destrutor: Garante que o AuraActor (corpo) seja destruído
   */
  virtual ~AuraWeapon();

  /**
   * OnUpdate da arma (vazio, pois a lógica está no AuraActor)
   */
  void OnUpdate(float deltaTime) override;

  /**
   * Chamado pelo Player para aplicar um upgrade
   */
  void LevelUp() override;

 private:
  // Ponteiro para o Ator (o "corpo") que esta arma criou
  class AuraActor* mAura;
};