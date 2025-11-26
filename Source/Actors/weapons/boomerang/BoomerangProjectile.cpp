// BoomerangProjectile.cpp
#include "BoomerangProjectile.h"
#include "../../../Components/Drawing/AnimatorComponent.h"  // <-- Usa um Sprite, não um Animator
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../../Game.h"
#include "../../../Math.h"  // Para ToRadians e Vector2::Perpendicular
#include "../../Aim.h"      // Necessário para pegar a posição da Mira
#include "../../Player.h"   // Necessário para pegar o Ator do Player e a Mira

BoomerangProjectile::BoomerangProjectile(Game* game, int width, int height)
    : Projectile(game, width, height)  // Chama a base
      ,
      mStartPosition(Vector2::Zero),
      mControlPoint(Vector2::Zero),
      mTimeElapsed(0.0f),
      mSpinSpeed(Math::ToRadians(720.0f))  // 720 graus/segundo
{
  // --- O Bumerangue cria seu próprio SpriteComponent ---
  // (mDrawComponent é 'protected' na classe base Projectile)
  if (mDrawComponent) {
    mDrawComponent->AddAnimation("boomerang", std::vector<int>{1});
    mDrawComponent->SetAnimation("boomerang");
  }
  mRigidBodyComponent->SetApplyGravity(false);

  // Define a escala do Ator
  SetScale(Vector2(static_cast<float>(width), static_cast<float>(height)));

  // Garante que o sprite comece invisível (pois começa "morto" no pool)
  mDrawComponent->SetVisible(false);
}

void BoomerangProjectile::Awake(Actor *owner,
                                const Vector2 &position,
                                float rotation,
                                float lifetime, const Vector2 &velocity, float damage, float areaScale) {
  // 1. Chama a base (define mShooter, Posição, Vida, Estado, Ativa
  // Colisor/Desenho)
  //    Passamos Vector2::Zero para a velocidade da base, pois não a usaremos.
    Projectile::Awake(owner, position, rotation, lifetime, velocity,damage, areaScale);

  // 2. Reseta o estado da curva de Bézier
  mTimeElapsed = 0.0f;
  mStartPosition = position;  // p0 = Posição de disparo
  mLifeTime = 1.f;

  // 3. Pega a direção da mira (como no seu snippet)
  Player* player = static_cast<Player*>(mShooter);
  Aim* aim = player->GetAim();  // Usa "Aim" como você corrigiu
  if (!aim) {
    Kill();
    return;
  }  // Segurança

  Vector2 direction = aim->GetPosition() - player->GetPosition();
  direction.Normalize();

  // 4. Calcula o Ponto de Controle (p1)
  mControlPoint = mStartPosition + (direction * mMaxRange);
}

void BoomerangProjectile::OnUpdate(float deltaTime) {
  mTimeElapsed += deltaTime;
  float t = mTimeElapsed / mLifeTime;

  // 2. Se o tempo acabou, se "mata" (retorna ao pool)
  if (t >= 1.0f) {
    Kill();
    return;
  }
  if (t <= 0.5f) {
    mState = BoomerangState::Extending;
  } else {
    mState = BoomerangState::Returning;
  }
  // 3. Pega os 3 pontos da curva de Bézier
  Vector2 p0 = mStartPosition;           // Ponto inicial (fixo)
  Vector2 p1 = mControlPoint;            // Ponto de controle (fixo)
  Vector2 p2 = mShooter->GetPosition();  // Ponto final (o Jogador, dinâmico!)

  // 4. Fórmula da Curva de Bézier Quadrática
  float one_minus_t = 1.0f - t;
  float x = (one_minus_t * one_minus_t * p0.x) +
            (2.0f * one_minus_t * t * p1.x) + (t * t * p2.x);
  float y = (one_minus_t * one_minus_t * p0.y) +
            (2.0f * one_minus_t * t * p1.y) + (t * t * p2.y);

  // 5. Define a Posição DIRETAMENTE
  SetPosition(Vector2(x, y));

  // 6. Atualiza a Rotação (Visual)
  float newRotation = GetRotation() + (mSpinSpeed * deltaTime);
  SetRotation(newRotation);
}

void BoomerangProjectile::OnHorizontalCollision(const float minOverlap,
                                                AABBColliderComponent* other) {
  ColliderLayer layer = other->GetLayer();

  if (layer == ColliderLayer::Blocks) {
    // Se bater em um bloco, começa a voltar imediatamente
    mState = BoomerangState::Returning;
  } else if (layer == ColliderLayer::Enemy) {
    // Causa dano no inimigo, mas NÃO para (continua voando)
    other->GetOwner()->Kill();
  } else if (layer == ColliderLayer::Player &&
             mState == BoomerangState::Returning) {
    // Se está voltando e toca no jogador, é "pego" (desativado)
    Kill();
  }
}

void BoomerangProjectile::OnVerticalCollision(const float minOverlap,
                                              AABBColliderComponent* other) {
  // Em top-down, a lógica é idêntica à horizontal
  OnHorizontalCollision(minOverlap, other);
}