#include "XPGem.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Components/Physics/AABBColliderComponent.h"
#include "../Components/Physics/RigidBodyComponent.h"
#include "../Game.h"
#include "Player.h"

XPGem::XPGem(Game* game, const Vector2& position, int xpValue)
    : Actor(game),
      mXPValue(xpValue),
      mMagnetRadius(150.0f),
      mMoveSpeed(400.0f)  // Speed when attracted
{
  SetPosition(position);
  SetScale(Vector2(16.0f, 16.0f));  // Small size

  // Using Coin sprite for now
  mAnimator =
      new AnimatorComponent(this, "../Assets/Sprites/Collectables/Coin.png",
                            "",  // No JSON data for now
                            16,  // width
                            16   // height
      );

  mRigidBody = new RigidBodyComponent(this);
  mRigidBody->SetApplyGravity(false);  // Floating gem

  mCollider = new AABBColliderComponent(this, 0, 0, 16, 16, ColliderLayer::XP,
                                        true  // IsTrigger
  );
}

void XPGem::OnUpdate(float deltaTime) {
  Actor::OnUpdate(deltaTime);

  // Magnet Logic
  const Player* player = GetGame()->GetPlayer();
  if (player) {
    Vector2 playerPos = player->GetPosition();
    Vector2 myPos = GetPosition();
    Vector2 diff = playerPos - myPos;

    float distSq = diff.LengthSq();
    if (distSq < mMagnetRadius * mMagnetRadius) {
      diff.Normalize();
      Vector2 velocity = diff * mMoveSpeed;
      mRigidBody->SetVelocity(velocity);
    } else {
      // Stop if player is out of range (optional, maybe keep drifting?)
      mRigidBody->SetVelocity(Vector2::Zero);
    }
  }
}

void XPGem::OnHorizontalCollision(float minOverlap,
                                  AABBColliderComponent* other) {
  if (other->GetLayer() == ColliderLayer::Player) {
    // Add XP to player
    auto player = dynamic_cast<Player*>(other->GetOwner());
    player->AddXP(mXPValue);
    SetState(ActorState::Destroy);
  }
}

void XPGem::OnVerticalCollision(float minOverlap,
                                AABBColliderComponent* other) {
  OnHorizontalCollision(minOverlap, other);
}
