#pragma once
#include <vector>
#include "../../../Components/Drawing/RectComponent.h"
#include "../../../Math.h"  // Para Vector4
#include "../../Actor.h"
// Forward declarations
class Player;
class AABBColliderComponent;

class AuraActor : public Actor {
 public:
  AuraActor(class Game* game, class Player* owner);

  // Sobrescreve métodos da base Actor
  void OnUpdate(float deltaTime) override;
  void OnHorizontalCollision(const float minOverlap,
                             AABBColliderComponent* other) override;
  void OnVerticalCollision(const float minOverlap,
                           AABBColliderComponent* other) override;

  // Chamado pelo "cérebro" (AuraWeapon) para aplicar upgrades
  void LevelUp();

 private:
  // Função auxiliar para aplicar dano (com timer de tick)
  void ApplyHit(Actor* enemy);

  Player* mOwnerPlayer;  // Ponteiro para o jogador que esta aura segue
  AABBColliderComponent* mCollider;
  RectComponent* mDrawComponent;

  // --- Estatísticas da Aura (controladas pelo LevelUp) ---
  int mLevel;
  float mDamage;
  float mAreaScale;    // Multiplicador de tamanho
  float mHitCooldown;  // Tempo (seg) entre "ticks" de dano no *mesmo* inimigo
  Vector4 mAuraColor;  // A cor semi-transparente (R, G, B, A)

  // --- Estado ---
  std::vector<Actor*> mEnemiesHitThisTick;  // Inimigos já atingidos neste tick
  float mHitTimer;                          // Timer para o próximo "tick"
};