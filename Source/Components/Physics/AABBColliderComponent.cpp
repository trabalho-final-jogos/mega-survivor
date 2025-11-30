//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AABBColliderComponent.h"
#include <cmath>
#include <cstdio>
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Managers/ColorPalette.h"
const char* ColliderLayerToString(ColliderLayer layer) {
  switch (layer) {
    case ColliderLayer::Player:
      return "Player";
    case ColliderLayer::Enemy:
      return "Enemy";
    case ColliderLayer::PowerUp:
      return "PowerUp";
    case ColliderLayer::Blocks:
      return "Blocks";
    default:
      return "Unknown";
  }
}
AABBColliderComponent::AABBColliderComponent(class Actor* owner,
                                             int dx,
                                             int dy,
                                             int w,
                                             int h,
                                             ColliderLayer layer,
                                             bool isStatic,
                                             int updateOrder)
    : Component(owner, updateOrder),
      mOffset(Vector2((float)dx, (float)dy)),
      mIsStatic(isStatic),
      mWidth(w),
      mHeight(h),
      mLayer(layer) {
  GetGame()->AddCollider(this);
}

AABBColliderComponent::~AABBColliderComponent() {
  GetGame()->RemoveCollider(this);
}

Vector2 AABBColliderComponent::GetMin() const {
  Vector2 center = mOwner->GetPosition() + mOffset;
  return Vector2(center.x - mWidth / 2.0f, center.y - mHeight / 2.0f);
}

Vector2 AABBColliderComponent::GetMax() const {
  Vector2 center = mOwner->GetPosition() + mOffset;
  return Vector2(center.x + mWidth / 2.0f, center.y + mHeight / 2.0f);
}

bool AABBColliderComponent::Intersect(const AABBColliderComponent& b) const {
  Vector2 thisMin = GetMin();
  Vector2 thisMax = GetMax();
  Vector2 otherMin = b.GetMin();
  Vector2 otherMax = b.GetMax();

  bool noOverlapX = thisMin.x >= otherMax.x || otherMin.x >= thisMax.x;
  bool noOverlapY = thisMin.y >= otherMax.y || otherMin.y >= thisMax.y;

  return !(noOverlapX || noOverlapY);
}

float AABBColliderComponent::GetMinVerticalOverlap(
    AABBColliderComponent* b) const {
  Vector2 thisMin = GetMin();
  Vector2 thisMax = GetMax();
  Vector2 otherMin = b->GetMin();
  Vector2 otherMax = b->GetMax();

  float overlapBottom = otherMax.y - thisMin.y;
  float overlapTop = otherMin.y - thisMax.y;

  return (std::abs(overlapTop) < std::abs(overlapBottom)) ? overlapTop
                                                          : overlapBottom;
}

float AABBColliderComponent::GetMinHorizontalOverlap(
    AABBColliderComponent* b) const {
  Vector2 thisMin = GetMin();
  Vector2 thisMax = GetMax();
  Vector2 otherMin = b->GetMin();
  Vector2 otherMax = b->GetMax();

  // Calcula sobreposição horizontal
  float overlapLeft = otherMax.x - thisMin.x;
  float overlapRight = otherMin.x - thisMax.x;

  if (overlapLeft >= 0.0f && overlapRight <= 0.0f)
    return (std::abs(overlapLeft) < std::abs(overlapRight)) ? overlapLeft
                                                            : overlapRight;

  return 0.0f;
}

float AABBColliderComponent::DetectVertialCollision(
    RigidBodyComponent* rigidBody) {
  if (mIsStatic)
    return 0.0f;

  auto colliders = GetGame()->GetColliders();
  for (auto other : colliders) {
    if (other == this || !other->IsEnabled())
      continue;
    ColliderLayer myLayer = GetLayer();
    ColliderLayer otherLayer = other->GetLayer();

    if ((myLayer == ColliderLayer::Enemy &&
         otherLayer == ColliderLayer::PowerUp) ||
        (myLayer == ColliderLayer::PowerUp &&
         otherLayer == ColliderLayer::Enemy)) {
      continue;
    }

    if (myLayer == ColliderLayer::PlayerProjectile &&
        otherLayer == ColliderLayer::PlayerProjectile) {
      continue;  // Pula (ignora colisão)
    }

    if ((myLayer == ColliderLayer::PlayerProjectile &&
         otherLayer == ColliderLayer::XP) ||
        (myLayer == ColliderLayer::XP &&
         otherLayer == ColliderLayer::PlayerProjectile)) {
      continue;
    }

    if (Intersect(*other)) {
      float overlap = GetMinVerticalOverlap(other);
      bool shouldResolvePhysics = true;

      if ((myLayer == ColliderLayer::PlayerProjectile) ||
          (otherLayer == ColliderLayer::PlayerProjectile)) {
        shouldResolvePhysics = false;
      }

      if ((myLayer == ColliderLayer::Player &&
           otherLayer == ColliderLayer::XP) ||
          (myLayer == ColliderLayer::XP &&
           otherLayer == ColliderLayer::Player)) {
        shouldResolvePhysics = false;
      }

      if ((myLayer == ColliderLayer::Enemy &&
           otherLayer == ColliderLayer::XP) ||
          (myLayer == ColliderLayer::XP &&
           otherLayer == ColliderLayer::Enemy)) {
        shouldResolvePhysics = false;
      }

      if ((myLayer == ColliderLayer::Enemy &&
           otherLayer == ColliderLayer::Enemy) ||
          (myLayer == ColliderLayer::Enemy &&
           otherLayer == ColliderLayer::Enemy)) {
        shouldResolvePhysics = false;
      }

      if ((myLayer == ColliderLayer::XP && otherLayer == ColliderLayer::XP) ||
          (myLayer == ColliderLayer::XP && otherLayer == ColliderLayer::XP)) {
        shouldResolvePhysics = false;
      }

      if (shouldResolvePhysics) {
        // Aplica a física (empurra e para o movimento)
        ResolveVerticalCollisions(rigidBody, overlap);
      }

      mOwner->OnVerticalCollision(overlap, other);
      return overlap;
    }
  }
  return 0.0f;
}

float AABBColliderComponent::DetectHorizontalCollision(
    RigidBodyComponent* rigidBody) {
  return DetectVertialCollision(rigidBody);
}

void AABBColliderComponent::ResolveHorizontalCollisions(
    RigidBodyComponent* rigidBody,
    const float minXOverlap) {
  if (std::abs(minXOverlap) < 0.01f)
    return;

  Vector2 pos = mOwner->GetPosition();
  pos.x += minXOverlap;
  mOwner->SetPosition(pos);

  Vector2 vel = rigidBody->GetVelocity();
  if ((minXOverlap > 0 && vel.x < 0) || (minXOverlap < 0 && vel.x > 0))
    vel.x = 0.0f;

  rigidBody->SetVelocity(vel);
}

void AABBColliderComponent::ResolveVerticalCollisions(
    RigidBodyComponent* rigidBody,
    const float minYOverlap) {
  if (minYOverlap == 0.0f)
    return;

  Vector2 pos = mOwner->GetPosition();
  pos.y += minYOverlap;
  mOwner->SetPosition(pos);

  Vector2 vel = rigidBody->GetVelocity();
  if ((minYOverlap > 0 && vel.y < 0) || (minYOverlap < 0 && vel.y > 0))
    vel.y = 0.0f;

  rigidBody->SetVelocity(vel);

  // if (minYOverlap < 0.0f)
  // mOwner->SetOnGround();
}

void AABBColliderComponent::SetSize(Vector2 size) {
  mWidth = size.x;
  mHeight = size.y;
}

void AABBColliderComponent::DebugDraw(class Renderer* renderer) {
  renderer->DrawRect(mOwner->GetPosition() + mOffset,
                     Vector2((float)mWidth, (float)mHeight),
                     mOwner->GetRotation(),
                     ColorPalette::GetInstance().GetColorAsVec4("Debug_Hitbox"),
                     mOwner->GetGame()->GetCameraPos(), RendererMode::LINES);
}

bool AABBColliderComponent::ShouldCollide(ColliderLayer a, ColliderLayer b) {
  SDL_Log("Checking collision: %d vs %d -> %s", a, b,
          ShouldCollide(a, b) ? "YES" : "NO");

  // I
  if (a == b)
    return false;

  if ((a == ColliderLayer::Enemy && b == ColliderLayer::PowerUp) ||
      (a == ColliderLayer::PowerUp && b == ColliderLayer::Enemy))
    return false;

  if ((a == ColliderLayer::Player && b == ColliderLayer::PowerUp) ||
      (a == ColliderLayer::PowerUp && b == ColliderLayer::Player))
    return true;

  if ((a == ColliderLayer::PowerUp && b == ColliderLayer::Blocks) ||
      (a == ColliderLayer::Blocks && b == ColliderLayer::PowerUp))
    return true;

  if ((a == ColliderLayer::Enemy && b == ColliderLayer::Blocks) ||
      (a == ColliderLayer::Blocks && b == ColliderLayer::Enemy))
    return true;

  if ((a == ColliderLayer::Player && b == ColliderLayer::Enemy) ||
      (a == ColliderLayer::Enemy && b == ColliderLayer::Player))
    return true;

  if ((a == ColliderLayer::Player && b == ColliderLayer::Blocks) ||
      (a == ColliderLayer::Blocks && b == ColliderLayer::Player))
    return true;

  return false;
}
