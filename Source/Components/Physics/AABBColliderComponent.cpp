//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AABBColliderComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include <cstdio>
#include <cmath>
const char* ColliderLayerToString(ColliderLayer layer)
{
    switch (layer)
    {
        case ColliderLayer::Player:   return "Player";
        case ColliderLayer::Enemy:    return "Enemy";
        case ColliderLayer::PowerUp:  return "PowerUp";
        case ColliderLayer::Blocks:   return "Blocks";
        default:                      return "Unknown";
    }
}
AABBColliderComponent::AABBColliderComponent(class Actor* owner, int dx, int dy, int w, int h,
        ColliderLayer layer, bool isStatic, int updateOrder)
        : Component(owner, updateOrder)
        , mOffset(Vector2((float)dx, (float)dy))
        , mIsStatic(isStatic)
        , mWidth(w)
        , mHeight(h)
        , mLayer(layer)
{
    GetGame()->AddCollider(this);
}

AABBColliderComponent::~AABBColliderComponent()
{
    GetGame()->RemoveCollider(this);
}

Vector2 AABBColliderComponent::GetMin() const
{
    Vector2 center = mOwner->GetPosition() + mOffset;
    return Vector2(center.x - mWidth / 2.0f, center.y - mHeight / 2.0f);
}

Vector2 AABBColliderComponent::GetMax() const
{
    Vector2 center = mOwner->GetPosition() + mOffset;
    return Vector2(center.x + mWidth / 2.0f, center.y + mHeight / 2.0f);
}

bool AABBColliderComponent::Intersect(const AABBColliderComponent& b) const
{
    Vector2 thisMin = GetMin();
    Vector2 thisMax = GetMax();
    Vector2 otherMin = b.GetMin();
    Vector2 otherMax = b.GetMax();

    // Muda os sinais > para >=, < para <= para permitir tocar na borda
    bool noOverlapX = thisMin.x >= otherMax.x || otherMin.x >= thisMax.x;
    bool noOverlapY = thisMin.y >= otherMax.y || otherMin.y >= thisMax.y;

    return !(noOverlapX || noOverlapY);
}

float AABBColliderComponent::GetMinVerticalOverlap(AABBColliderComponent* b) const
{
    Vector2 thisMin = GetMin();
    Vector2 thisMax = GetMax();
    Vector2 otherMin = b->GetMin();
    Vector2 otherMax = b->GetMax();

    float overlapBottom = otherMax.y - thisMin.y; // nosso topo vs fundo do outro
    float overlapTop    = otherMin.y - thisMax.y; // nosso fundo vs topo do outro

    return (std::abs(overlapTop) < std::abs(overlapBottom)) ? overlapTop : overlapBottom;
}

float AABBColliderComponent::GetMinHorizontalOverlap(AABBColliderComponent* b) const
{
    Vector2 thisMin = GetMin();
    Vector2 thisMax = GetMax();
    Vector2 otherMin = b->GetMin();
    Vector2 otherMax = b->GetMax();

    // Calcula sobreposição horizontal
    float overlapLeft  = otherMax.x - thisMin.x;   // positivo → empurra para a direita
    float overlapRight = otherMin.x - thisMax.x;  // negativo → empurra para a esquerda

    // Considera toque na borda como colisão mínima
    if (overlapLeft >= 0.0f && overlapRight <= 0.0f)
        return (std::abs(overlapLeft) < std::abs(overlapRight)) ? overlapLeft : overlapRight;

    return 0.0f;
}

float AABBColliderComponent::DetectVertialCollision(RigidBodyComponent *rigidBody)
{
    if (mIsStatic) return 0.0f;

    auto colliders = GetGame()->GetColliders();
    for (auto other : colliders)
    {
        if (other == this || !other->IsEnabled()) continue;
        ColliderLayer myLayer = GetLayer(); // Pega a NOSSA camada
        ColliderLayer otherLayer = other->GetLayer(); // Pega a camada DO OUTRO

        // Regra: Inimigos (Enemy) ignoram PowerUps e vice-versa
        if ((myLayer == ColliderLayer::Enemy && otherLayer == ColliderLayer::PowerUp) ||
            (myLayer == ColliderLayer::PowerUp && otherLayer == ColliderLayer::Enemy))
        {
            continue; // Pula para o próximo colisor, ignorando esta colisão
        }

        if (Intersect(*other))
        {
            float overlap = GetMinVerticalOverlap(other);
            ResolveVerticalCollisions(rigidBody, overlap);
            mOwner->OnVerticalCollision(overlap, other);
            return overlap;
        }
    }
    return 0.0f;
}

float AABBColliderComponent::DetectHorizontalCollision(RigidBodyComponent *rigidBody)
{
    if (mIsStatic) return 0.0f;

    auto colliders = GetGame()->GetColliders();
    for (auto other : colliders)
    {
        if (other == this || !other->IsEnabled()) continue;

        ColliderLayer myLayer = GetLayer(); // Pega a NOSSA camada
        ColliderLayer otherLayer = other->GetLayer(); // Pega a camada DO OUTRO

        // Regra: Inimigos (Enemy) ignoram PowerUps e vice-versa
        if ((myLayer == ColliderLayer::Enemy && otherLayer == ColliderLayer::PowerUp) ||
            (myLayer == ColliderLayer::PowerUp && otherLayer == ColliderLayer::Enemy))
        {
            continue; // Pula para o próximo colisor, ignorando esta colisão
        }

        if (Intersect(*other))
        {
            float overlap = GetMinHorizontalOverlap(other);

            // DEBUG
            Vector2 thisMin = GetMin();
            Vector2 thisMax = GetMax();
            Vector2 otherMin = other->GetMin();
            Vector2 otherMax = other->GetMax();

            ResolveHorizontalCollisions(rigidBody, overlap);
            mOwner->OnHorizontalCollision(overlap, other);
            return overlap;
        }
    }
    return 0.0f;
}

void AABBColliderComponent::ResolveHorizontalCollisions(RigidBodyComponent *rigidBody, const float minXOverlap)
{
    // Só resolve se houver colisão real
    if (std::abs(minXOverlap) < 0.01f)
        return;

    Vector2 pos = mOwner->GetPosition();
    pos.x += minXOverlap;
    mOwner->SetPosition(pos);

    Vector2 vel = rigidBody->GetVelocity();
    // Zera velocidade apenas na direção da colisão
    if ((minXOverlap > 0 && vel.x < 0) || (minXOverlap < 0 && vel.x > 0))
        vel.x = 0.0f;

    rigidBody->SetVelocity(vel);
}

void AABBColliderComponent::ResolveVerticalCollisions(RigidBodyComponent *rigidBody, const float minYOverlap)
{
    if (minYOverlap == 0.0f) return;

    Vector2 pos = mOwner->GetPosition();
    pos.y += minYOverlap;
    mOwner->SetPosition(pos);

    Vector2 vel = rigidBody->GetVelocity();
    if ((minYOverlap > 0 && vel.y < 0) || (minYOverlap < 0 && vel.y > 0))
        vel.y = 0.0f;

    rigidBody->SetVelocity(vel);

    if (minYOverlap < 0.0f)
        mOwner->SetOnGround();
}

void AABBColliderComponent::SetSize(Vector2 size) {
    mWidth = size.x;
    mHeight = size.y;
}

void AABBColliderComponent::DebugDraw(class Renderer *renderer)
{
    renderer->DrawRect(mOwner->GetPosition() + mOffset,
                       Vector2((float)mWidth, (float)mHeight),
                       mOwner->GetRotation(),
                       Color::Green,
                       mOwner->GetGame()->GetCameraPos(),
                       RendererMode::LINES);
}

bool AABBColliderComponent::ShouldCollide(ColliderLayer a, ColliderLayer b)
{
    SDL_Log("Checking collision: %d vs %d -> %s", a, b, ShouldCollide(a,b) ? "YES" : "NO");

    // I
    // mpede colisão de um layer com ele mesmo
    if (a == b) return false;

    // Evita colisão entre inimigos e power-ups (Goomba x Cogumelo)
    if ((a == ColliderLayer::Enemy && b == ColliderLayer::PowerUp) ||
        (a == ColliderLayer::PowerUp && b == ColliderLayer::Enemy))
        return false;

    // Permite colisão entre Player e PowerUp (Mario pega cogumelo)
    if ((a == ColliderLayer::Player && b == ColliderLayer::PowerUp) ||
        (a == ColliderLayer::PowerUp && b == ColliderLayer::Player))
        return true;

    // Permite colisão entre PowerUp e blocos (para andar e quicar)
    if ((a == ColliderLayer::PowerUp && b == ColliderLayer::Blocks) ||
        (a == ColliderLayer::Blocks && b == ColliderLayer::PowerUp))
        return true;

    // Permite colisão entre inimigos e blocos
    if ((a == ColliderLayer::Enemy && b == ColliderLayer::Blocks) ||
        (a == ColliderLayer::Blocks && b == ColliderLayer::Enemy))
        return true;

    // Permite colisão entre Player e inimigos
    if ((a == ColliderLayer::Player && b == ColliderLayer::Enemy) ||
        (a == ColliderLayer::Enemy && b == ColliderLayer::Player))
        return true;

    // Permite colisão entre Player e blocos
    if ((a == ColliderLayer::Player && b == ColliderLayer::Blocks) ||
        (a == ColliderLayer::Blocks && b == ColliderLayer::Player))
        return true;

    // Por padrão, não colide
    return false;
}
