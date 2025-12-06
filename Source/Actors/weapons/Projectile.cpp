#include "Projectile.h"
#include "../../Components/Drawing/AnimatorComponent.h"
#include "../../Components/Drawing/DrawComponent.h"
#include "../../Components/Physics/AABBColliderComponent.h"
#include "../../Components/Physics/RigidBodyComponent.h"
#include "../../Game.h"

Projectile::Projectile(class Game* game, int width, int height)
    : Actor(game),
      mDrawComponent(nullptr),
      mRigidBodyComponent(nullptr),
      mColliderComponent(nullptr),
      mIsDead(true),
      mLifeTime(1.0f),
      mShooter(nullptr),
      mDamage(1),
      mWidth(width),
      mHeight(height) {
  // A classe base pode criar um RectComponent simples como placeholder
  mDrawComponent =
      new AnimatorComponent(this, "../Assets/Sprites/weapons/texture.png",
                            "../Assets/Sprites/weapons/texture.json",
                            Game::TILE_SIZE, Game::TILE_SIZE);
  mRigidBodyComponent =
      new RigidBodyComponent(this, 1.0f, 0.0f, false);  // Gravidade false
  mColliderComponent = new AABBColliderComponent(
      this, 0, 0, width, height,
      ColliderLayer::PlayerProjectile);  // Camada correta

  SetState(ActorState::Paused);
  mDrawComponent->SetVisible(false);
  mColliderComponent->SetEnabled(false);
}

void Projectile::Kill() {
  mIsDead = true;
  SetState(ActorState::Paused);
  mDrawComponent->SetVisible(false);
  mColliderComponent->SetEnabled(false);
  mRigidBodyComponent->SetVelocity(Vector2::Zero);
}

void Projectile::Awake(Actor* owner,
                       const Vector2& position,
                       float rotation,
                       float lifetime,
                       const Vector2& velocity,
                       float damage,
                       float areaScale) {
  mShooter = owner;
  mLifeTime = lifetime;
  mIsDead = false;
  mDamage = damage;
  SetState(ActorState::Active);
  mDrawComponent->SetVisible(true);
  mColliderComponent->SetEnabled(true);
  SetPosition(position);
  SetRotation(rotation);
  mRigidBodyComponent->SetVelocity(velocity);
  SDL_Log("Projectile awake %f, %f, %f", mWidth, mWidth, areaScale);
  SetScale(Vector2(areaScale * mWidth, areaScale * mHeight));
}

void Projectile::OnUpdate(float deltaTime) {
  Actor::OnUpdate(
      deltaTime);  // Necessário para atualizar componentes (RigidBody)

  mLifeTime -= deltaTime;
  if (mLifeTime <= 0) {
    Kill();
    return;
  }
}

// Comportamento de colisão BASE: Destruir ao bater em blocos
void Projectile::OnHorizontalCollision(const float minOverlap,
                                       AABBColliderComponent* other) {
  if (other->GetLayer() == ColliderLayer::Blocks) {
    Kill();
  }
}

void Projectile::OnVerticalCollision(const float minOverlap,
                                     AABBColliderComponent* other) {
  OnHorizontalCollision(minOverlap, other);  // Mesma lógica
}