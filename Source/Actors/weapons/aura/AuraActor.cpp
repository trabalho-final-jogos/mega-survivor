#include "AuraActor.h"
#include <algorithm>
#include "../../../Components/Drawing/AnimatorComponent.h"  // <-- Usa um Sprite, não um Animator
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../../Game.h"
#include "../../../Math.h"  // Para ToRadians e Vector2::Perpendicular
#include "../../../Renderer/Renderer.h"
#include "../../Aim.h"     // Necessário para pegar a posição da Mira
#include "../../Player.h"  // Necessário para pegar o Ator do Player e a Mira
#include "../../Enemy.h"

AuraActor::AuraActor(Game* game, Player* owner)
    : Actor(game)  // O Actor base se registra no Game
      ,
      mOwnerPlayer(owner),
      mCollider(nullptr),
      mLevel(0)
      // Cor Padrão: Azul claro, 20% Opaco (Alpha = 0.2f)
      ,
      mAuraColor(1.0f, 0.4f, 0.0f, 0.2f),
      mHitTimer(0.0f),
      mIsDamageTick(false) {

  mCollider = new AABBColliderComponent(
      this, 0, 0, Game::TILE_SIZE * 2, Game::TILE_SIZE * 2,  // Tamanho base
      ColliderLayer::PlayerProjectile,  // Age como um projétil
      false);                           // Não é estático

  mCollider->SetEnabled(true);
  mDrawComponent = new RectComponent(this,
                                     Game::TILE_SIZE * 1.5,  // largura
                                     Game::TILE_SIZE * 1.5,  // altura
                                     RendererMode::TRIANGLES, 99);

  mDrawComponent->SetColor(mAuraColor);  // Define a cor inicial
  LevelUp();
}

void AuraActor::LevelUp() {
  mLevel++;
  SDL_Log("Aura subiu para o Nível %d!", mLevel);

  // Define estatísticas baseadas no nível
  switch (mLevel) {
    case 1:
      mAreaScale = 1.5f;  // 1.5x o tamanho do Player
      mDamage = 5.0f;
      mHitCooldown = 1.0f;  // 1 tick de dano por segundo
      mAuraColor = Vector4(1.0f, 0.4f, 0.0f, 0.5f);
      break;
    case 2:
      mAreaScale = 2.0f;  // 2.0x o tamanho
      mDamage = 8.0f;
      mAuraColor = Vector4(1.0f, 0.3f, 0.0f, 0.5f);
      break;
    case 3:
      mAreaScale = 2.5f;
      mDamage = 12.0f;
      mHitCooldown = 0.8f;  // Ticks mais rápidos
      mAuraColor = Vector4(1.0f, 0.3f, 0.0f, 0.5f);
      break;
    default:  // Níveis > 3
      mAreaScale += 0.5f;
      mDamage += 5.0f;
      break;
  }

  float baseSize = Game::TILE_SIZE;

  Vector2 newSize(baseSize * mAreaScale, baseSize * mAreaScale);

  SetScale(newSize);

  if (mCollider) {
    mCollider->SetSize(newSize);
  }
  if (mDrawComponent) {
    mDrawComponent->SetColor(mAuraColor);
  }
}

void AuraActor::OnUpdate(float deltaTime) {
  Actor::OnUpdate(deltaTime);
  SDL_Log("HIT %f", mHitTimer);
  SDL_Log("HIT %f", mHitCooldown);

  // Se o Player (dono) for destruído, destrói a aura também
  if (!mOwnerPlayer || mOwnerPlayer->GetState() != ActorState::Active) {
    SetState(ActorState::Destroy);
    return;
  }

  // 1. Segue o jogador
  SetPosition(mOwnerPlayer->GetPosition());

  // 2. Lógica do Timer de "Tick" de Dano
  mHitTimer -= deltaTime;
  mIsDamageTick = false;
  if (mHitTimer <= 0.0f) {
    mIsDamageTick = true;
    mHitTimer = mHitCooldown;     // Reseta o timer
    mEnemiesHitThisTick.clear();  // Limpa a lista de inimigos já atingidos
  }

  auto& colliders = GetGame()->GetColliders();

    // Percorre a lista para ver quem estamos a tocar
    for (auto other : colliders)
    {
        // Ignora a si mesmo, o Player e objetos desativados
        if (other == mCollider || !other->IsEnabled()) continue;

        // Otimização: Só nos importamos com Inimigos
        if (other->GetLayer() != ColliderLayer::Enemy) continue;

        // Usa a função Intersect que já existe no componente
        if (mCollider->Intersect(*other))
        {
            // ENCONTRAMOS UM INIMIGO DENTRO DA AURA!
            // Não precisamos de "Resolver" (empurrar), apenas aplicar a lógica.

            Actor* enemyActor = other->GetOwner();
            if (enemyActor)
            {
                ApplyHit(enemyActor);
            }
        }
    }
}

// Função auxiliar para aplicar dano (com timer de tick)
void AuraActor::ApplyHit(Actor* enemy) {
    SDL_Log("AAAAAAAAAAAAAAAAAAA");

  if (!mIsDamageTick)
  {
    return;
  }

  // Verifica se já atingimos este inimigo neste tick
  if (std::find(mEnemiesHitThisTick.begin(), mEnemiesHitThisTick.end(),
                enemy) == mEnemiesHitThisTick.end()) {

    Enemy* enemy_cast = dynamic_cast<Enemy*>(enemy);
    // Se não atingimos, aplica o dano e adiciona à lista
    SDL_Log("BBBBBBBBBBBB %f", mDamage);
    enemy_cast->TakeDamage(mDamage);
    mEnemiesHitThisTick.push_back(enemy);
  }
}

void AuraActor::OnHorizontalCollision(const float minOverlap,
                                      AABBColliderComponent* other) {}

void AuraActor::OnVerticalCollision(const float minOverlap,
                                    AABBColliderComponent* other) {}