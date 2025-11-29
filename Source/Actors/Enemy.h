#pragma once
#include "Actor.h"

class Enemy : public Actor {
 public:
  Enemy(class Game* game);

 private:
  int mHealth;
  uint16_t mXPDrop;
  class RigidBodyComponent* mRigidBody;
  class AnimatorComponent* mDrawComponent;
};