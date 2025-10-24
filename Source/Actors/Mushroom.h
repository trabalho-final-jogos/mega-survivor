//
// Created by filipe on 10/22/25.
//

// Mushroom.h
#pragma once
#include "Actor.h"
#include "../Math.h"
#include "../Game.h"

// Forward declarations
class RigidBodyComponent;
class AnimatorComponent; // Ou SpriteComponent se preferir
class AABBColliderComponent;

class Mushroom : public Actor
{
public:
    Mushroom(class Game* game, const Vector2& initialPos);

    void OnUpdate(float deltaTime) override;
    void OnHorizontalCollision(float minOverlap, AABBColliderComponent* other) override;
    void OnVerticalCollision( float minOverlap,  AABBColliderComponent* other) override;

private:
    RigidBodyComponent* mRigidBody;
    // AnimatorComponent* mAnimator; // Desnecessário se for só 1 sprite

    float mWalkSpeed;

    // Para a animação de "sair do bloco"
    bool mIsPoppingUp;
    float mPopUpDistance; // Distância total que já subiu
    const float POP_UP_TARGET = 16.0f; // Quanto deve subir
    const float POP_UP_SPEED = 50.0f; // Velocidade de subida
     float WALK_SPEED = 40.0f;
    float mPopStartY;   // y inicial quando criado
    float mPopTargetY;
};