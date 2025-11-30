#include "Bat.h"
#include "../../Components/Drawing/AnimatorComponent.h"  // ou SpriteComponent
#include "../../Components/Physics/AABBColliderComponent.h"
#include "../../Components/Physics/RigidBodyComponent.h"
#include "../../Game.h"

Bat::Bat(Game* game) : Enemy(game, 1, 10) {
  SetScale(Vector2(Game::TILE_SIZE, Game::TILE_SIZE));

  // 1. Física (SEM gravidade)
  mRigidBody = new RigidBodyComponent(this);
  mRigidBody->SetApplyGravity(false);

  // 2. Colisão
  new AABBColliderComponent(this, 0, 0, 24, 24, ColliderLayer::Enemy, false);

  mDrawComponent = new AnimatorComponent(
      this, "../Assets/Sprites/enemy4/enemy4.png",
      "../Assets/Sprites/enemy4/enemy4.json", Game::TILE_SIZE, Game::TILE_SIZE);
  mDrawComponent->SetEnabled(true);
  mDrawComponent->SetVisible(true);
  mDrawComponent->AddAnimation("walk", std::vector<int>{0, 1, 2, 1});
  mDrawComponent->SetAnimation("walk");
  mDrawComponent->SetAnimFPS(3.0f);
}

void Bat::SetFixedVelocity(const Vector2& velocity) {
  if (mRigidBody) {
    mRigidBody->SetVelocity(velocity);
  }
}

void Bat::OnUpdate(float deltaTime) {
  Actor::OnUpdate(deltaTime);

  // O Bat NÃO persegue o player. Ele apenas segue em frente.
  // A única lógica aqui é verificar se saiu da tela para se destruir.

  Vector2 pos = GetPosition();
  Vector2 camPos = GetGame()->GetCameraPos();
  float buffer = 200.0f;  // Margem grande para garantir que cruzou a tela toda

  // Se saiu muito longe da câmera, morre
  if (pos.x < camPos.x - buffer ||
      pos.x > camPos.x + Game::VIRTUAL_WIDTH + buffer ||
      pos.y < camPos.y - buffer ||
      pos.y > camPos.y + Game::VIRTUAL_HEIGHT + buffer) {
    SetState(ActorState::Destroy);
  }
}

void Bat::SetStats(const float health) {
  mHealth = health;
}