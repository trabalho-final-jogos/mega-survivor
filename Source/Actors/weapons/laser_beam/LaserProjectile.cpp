// LaserProjectile.cpp
#include "LaserProjectile.h"
#include <vector>
#include <algorithm>
#include "../../../Game.h"
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Drawing/AnimatorComponent.h" // <-- Usa um Sprite, não um Animator
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../Player.h" // Necessário para pegar o Ator do Player e a Mira
#include "../../Aim.h"  // Necessário para pegar a posição da Mira
#include "../../../Math.h" // Para ToRadians e Vector2::Perpendicular
#include "../../Enemy.h"

LaserProjectile::LaserProjectile(Game* game, int width, int height)
    : Projectile(game, width, height) // Chama a base (que carrega o spritesheet)
    , mBouncesLeft(0)
    , mHitCooldown(0.5f) // Dá dano a cada meio segundo
    , mHitTimer(0.0f)
{
    if (mDrawComponent)
    {
        // !! Substitua '8' pelo índice do seu "Sprite de Laser" no JSON !!
        mDrawComponent->AddAnimation("fly_laser", std::vector<int>{10});
        mDrawComponent->SetAnimFPS(10.0f);
    }
    mColliderComponent->SetSize(Vector2(28, 8));
}

void LaserProjectile::Awake(Actor* owner, const Vector2 &position, float rotation,
                           float lifetime, const Vector2& velocity,
                           float damage, float areaScale)
{
    // Chama a base (ativa o ator, define stats, etc.)
    Projectile::Awake(owner, position, rotation, lifetime, velocity, damage, areaScale);

    if (mDrawComponent)
    {
        mDrawComponent->SetAnimation("fly_laser");
    }

    mEnemiesHit.clear();
    mHitTimer = 0.0f;
}

void LaserProjectile::OnUpdate(float deltaTime)
{
    // 1. Chama o Update da base (que verifica o LifeTime e chama Kill() se expirar)
    Projectile::OnUpdate(deltaTime);
    if (IsDead()) { return; }

    // --- LÓGICA DE RICOCHETE NA CÂMERA ---

    Vector2 pos = GetPosition();
    Vector2 vel = mRigidBodyComponent->GetVelocity();
    Vector2 camPos = GetGame()->GetCameraPos(); // Pega o canto (0,0) da câmera

    // Pega as bordas da tela (em coordenadas do mundo)
    float screenLeft = camPos.x;
    float screenRight = camPos.x + Game::VIRTUAL_WIDTH;
    float screenTop = camPos.y;
    float screenBottom = camPos.y + Game::VIRTUAL_HEIGHT;

    bool bounced = false; // Flag para saber se ricocheteou

    Vector2 currentVelocity = mRigidBodyComponent->GetVelocity();

    // Evita dividir por zero ou calcular rotação para um vetor nulo
    if (currentVelocity.LengthSq() > 0.001f) // Se houver movimento significativo
    {
        // Calcula o ângulo em radianos (atan2(y, x))
        // Math::VectorToAngle() no seu 'Math.h' pode já fazer isso.
        float angleRad = std::atan2(currentVelocity.y, currentVelocity.x);

        // Assumindo que você tem uma função no seu Math.h
        //float angleRad = Math::VectorToAngle(currentVelocity);

        // Define a rotação do Ator. O Sprite/AnimatorComponent irá usá-la.
        SetRotation(angleRad);
    }

    // 2. Verifica as bordas Horizontais (Esquerda/Direita)
    if (pos.x <= screenLeft)
    {
        pos.x = screenLeft; // "Empurra" de volta para dentro
        vel.x *= -1.0f;     // Inverte a velocidade X
        bounced = true;
    }
    else if (pos.x >= screenRight)
    {
        pos.x = screenRight; // "Empurra" de volta para dentro
        vel.x *= -1.0f;      // Inverte a velocidade X
        bounced = true;
    }

    // 3. Verifica as bordas Verticais (Cima/Baixo)
    if (pos.y <= screenTop)
    {
        pos.y = screenTop;
        vel.y *= -1.0f; // Inverte a velocidade Y
        bounced = true;
    }
    else if (pos.y >= screenBottom)
    {
        pos.y = screenBottom;
        vel.y *= -1.0f; // Inverte a velocidade Y
        bounced = true;
    }

    // 4. Se ricocheteou, gasta um "bounce" e atualiza a física
    if (bounced)
    {
        mEnemiesHit.clear();
        mBouncesLeft--; // Gasta um ricochete

        SetPosition(pos); // Define a posição corrigida
        mRigidBodyComponent->SetVelocity(vel); // Define a velocidade invertida

        if (mBouncesLeft < 0)
        {
            Kill(); // Se acabaram os ricochetes, "morre" (volta ao pool)
        }
    }
    mHitTimer -= deltaTime;
    if (mHitTimer <= 0.0f)
    {
        // O tempo passou! Limpa a lista para poder dar dano de novo.
        mEnemiesHit.clear();
        mHitTimer = mHitCooldown; // Reinicia o timer
    }
}

// Lógica de colisão da Laser (PIERCE - Atravessa inimigos)
void LaserProjectile::OnHorizontalCollision(const float minOverlap, AABBColliderComponent* other)
{
    ColliderLayer otherLayer = other->GetLayer();

    if (otherLayer == ColliderLayer::Enemy)
    {
        Actor* enemyActor = other->GetOwner();
        bool alreadyHit = std::find(mEnemiesHit.begin(), mEnemiesHit.end(), enemyActor) != mEnemiesHit.end();

        if (!alreadyHit) {
            Enemy* enemy = dynamic_cast<Enemy*>(enemyActor);

            if (enemy)
            {
                enemy->TakeDamage(this->GetDamage());
                mEnemiesHit.push_back(enemyActor);
            }
        }
    }

}

void LaserProjectile::OnVerticalCollision(const float minOverlap, AABBColliderComponent* other)
{
    OnHorizontalCollision(minOverlap, other); // Mesma lógica
}