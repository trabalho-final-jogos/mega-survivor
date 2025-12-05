#pragma once
#include "../../Math.h"
#include "../Actor.h"

class Projectile : public Actor {
 public:
  // O construtor ainda recebe as dimensões padrão do colisor/desenho
  Projectile(class Game* game, int width, int height);

  // Destrutor virtual (importante para classes base)
  virtual ~Projectile() {}

  // Kill() agora é virtual para que classes filhas (como Bullet)
  // possam adicionar lógica extra (como retornar ao pool)
  virtual void Kill();

  // "Acorda" o projétil do pool
  virtual void Awake(Actor* owner,
                     const Vector2& position,
                     float rotation,
                     float lifetime,
                     const Vector2& velocity,
                     float damage = 1.0f,
                     float areaScale = 1.0f);

  // Update padrão (verifica o tempo de vida)
  void OnUpdate(float deltaTime) override;

  float GetDamage() const { return mDamage; }

  // Colisões virtuais (para classes filhas definirem o dano)
  virtual void OnHorizontalCollision(const float minOverlap,
                                     AABBColliderComponent* other) override;
  virtual void OnVerticalCollision(const float minOverlap,
                                   AABBColliderComponent* other) override;

  bool IsDead() const { return mIsDead; }

 protected:  // Mude para protected para que as classes filhas possam acessá-los
  class Actor* mShooter;
  class AnimatorComponent* mDrawComponent;
  class RigidBodyComponent* mRigidBodyComponent;
  class AABBColliderComponent* mColliderComponent;
  bool mIsDead;
  float mLifeTime;
  float mDamage;
};