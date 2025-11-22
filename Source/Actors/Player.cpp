//
// Created by Lucas N. Ferreira on 03/08/23.
//

#include "Player.h"

#include <bits/fs_fwd.h>

#include <typeinfo>
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Components/ParticleSystemComponent.h"
#include "../Components/Physics/AABBColliderComponent.h"
#include "../Components/Physics/RigidBodyComponent.h"
#include "../Game.h"
#include "Block.h"
#include "weapons/WeaponType.h"
#include "weapons/boomerang/BoomerangWeapon.h"
#include "weapons/ice_gun/IceGun.h"
#include "weapons/main_gun/MainGun.h"

Player::Player(Game* game, const float forwardSpeed, const float jumpSpeed)
    : Actor(game),
      mIsRunning(false),
      mIsDead(false),
      mForwardSpeed(forwardSpeed),
      mJumpSpeed(jumpSpeed),
      mIsBig(false),
      mIsInvulnerable(false),
      mInvulnerabilityTimer(0.0f),
      mAimer(nullptr) {
  name = "Mario";
  SetScale(Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
  mDrawComponent = new AnimatorComponent(
      this, "../Assets/Sprites/player/player.png",
      "../Assets/Sprites/player/player.json", Game::TILE_SIZE, Game::TILE_SIZE);
  mDrawComponent->SetFlipHorizontal(true);

  mDrawComponent->AddAnimation("idle", std::vector<int>{0});
  mDrawComponent->AddAnimation("walk", std::vector<int>{1, 2, 3});

  mDrawComponent->SetAnimation("idle");
  mDrawComponent->SetAnimFPS(10.0f);

  mRigidBodyComponent = new RigidBodyComponent(this);

  mColliderComponent = new AABBColliderComponent(
      this, 0, 0, Game::TILE_SIZE, Game::TILE_SIZE, ColliderLayer::Player);
  SetOnGround();

  mAimer = new Aim(this->GetGame(), this);
  new MainGun(this);
  new BoomerangWeapon(this);
  new IceGun(this);
}

void Player::OnProcessInput(const uint8_t* state) {
  if (!mRigidBodyComponent) {
    return;
  }

  mIsRunning = false;
  Vector2 velocity = Vector2::Zero;  // Começa com velocidade zero

  // --- NOVA LÓGICA TOP-DOWN (W/A/S/D) ---

  // Eixo Y (Norte/Sul)
  if (state[SDL_SCANCODE_W]) {
    velocity.y = -mForwardSpeed;  // Y negativo é para CIMA
    mIsRunning = true;
  }
  if (state[SDL_SCANCODE_S]) {
    velocity.y = mForwardSpeed;  // Y positivo é para BAIXO
    mIsRunning = true;
  }

  // Eixo X (Leste/Oeste)
  if (state[SDL_SCANCODE_D]) {
    velocity.x = mForwardSpeed;
    mIsRunning = true;
    // if(mDrawComponent) { mDrawComponent->SetFlipHorizontal(true); }
  }
  if (state[SDL_SCANCODE_A]) {
    velocity.x = -mForwardSpeed;
    mIsRunning = true;
    // if(mDrawComponent) { mDrawComponent->SetFlipHorizontal(false); }
  }

  // Normalizar velocidade diagonal (opcional, mas recomendado)
  // Se estiver andando na diagonal, a velocidade será > mForwardSpeed.
  // Isso corrige para que a velocidade diagonal seja a mesma da
  // horizontal/vertical.
  if (velocity.LengthSq() > mForwardSpeed * mForwardSpeed) {
    velocity.Normalize();
    velocity *= mForwardSpeed;
  }

  // --- REMOVER LÓGICA DE PULO ---
  // if (state[SDL_SCANCODE_SPACE] && mIsOnGround) { ... } // REMOVIDO

  // --- APLICAÇÃO FINAL ---
  // Define a velocidade diretamente (controle arcade)
  mRigidBodyComponent->SetVelocity(velocity);
}
void Player::OnUpdate(float deltaTime) {
  if (mIsInvulnerable) {
    mInvulnerabilityTimer -= deltaTime;

    if (mDrawComponent) {
      bool show = static_cast<int>(mInvulnerabilityTimer * 10.0f) % 2 == 0;
      mDrawComponent->SetVisible(show);
    }

    if (mInvulnerabilityTimer <= 0.0f) {
      mIsInvulnerable = false;
      if (mDrawComponent) {
        mDrawComponent->SetVisible(true);
      }
      SDL_Log("Mario nao esta mais invulneravel.");
    }
  }
  if (mRigidBodyComponent) {
    Vector2 velocity = mRigidBodyComponent->GetVelocity();

    if (!Math::NearlyZero(velocity.y)) {
      // SetOffGround();
    }
  }
  // Pega a posição atual do Mario (que já foi atualizada pela física)
  Vector2 pos = GetPosition();

  // Pega as "meias-dimensões" do Mario
  // (Assumindo que a escala do Ator é o tamanho em pixels)
  float halfWidth = GetScale().x / 2.0f;
  float halfHeight =
      GetScale().y / 2.0f;  // Use GetScale().y se Mario Grande/Pequeno

  // Calcula os limites do mundo
  float levelWidth = Game::LEVEL_WIDTH * Game::TILE_SIZE;
  float levelHeight = Game::LEVEL_HEIGHT * Game::TILE_SIZE;

  // Trava a posição X do Mario
  pos.x =
      std::max(halfWidth, pos.x);  // Trava na borda Esquerda (0.0 + halfWidth)
  pos.x = std::min(levelWidth - halfWidth, pos.x);  // Trava na borda Direita

  // Trava a posição Y do Mario
  pos.y = std::max(halfHeight,
                   pos.y);  // Trava na borda Superior (0.0 + halfHeight)
  pos.y = std::min(levelHeight - halfHeight, pos.y);  // Trava na borda Inferior

  // Define a posição corrigida (travada)
  SetPosition(pos);

  // --- NOVA LÓGICA DE FLIP (BASEADA NO MOUSE) ---
  if (mDrawComponent)  // mAnimator é o nome que usamos para o mDrawComponent
  {
    // Pega a posição do mouse no MUNDO
    Vector2 mouseWorldPos = GetGame()->GetMousePos();

    // Pega a posição do Mario no MUNDO
    Vector2 marioPos = GetPosition();

    // Compara o X do mouse com o X do Mario
    if (mouseWorldPos.x > marioPos.x) {
      // Mouse está à direita -> vira para a direita
      mDrawComponent->SetFlipHorizontal(true);
    } else if (mouseWorldPos.x < marioPos.x) {
      // Mouse está à esquerda -> vira para a esquerda
      mDrawComponent->SetFlipHorizontal(false);
    }
    // (Se for igual, mantém o flip anterior)
  }

  ManageAnimations();
}

void Player::ManageAnimations() {
  if (!mDrawComponent)
    return;

  if (mIsDead) {
    mDrawComponent->SetAnimation("dead");
    return;
  }

  const Vector2 vel = mRigidBodyComponent->GetVelocity();

  if (!mIsOnGround) {
    mDrawComponent->SetAnimation("jump");
  } else if (mIsRunning) {
    mDrawComponent->SetAnimation("walk");
  } else {
    mDrawComponent->SetAnimation("idle");
  }
}

void Player::Kill() {
  mDrawComponent->SetAnimation("dead");
  mIsDead = true;
  mRigidBodyComponent->SetVelocity(Vector2::Zero);
  mRigidBodyComponent->SetEnabled(false);
  mColliderComponent->SetEnabled(false);
}

void Player::OnHorizontalCollision(const float minOverlap,
                                   AABBColliderComponent* other) {
  ColliderLayer otherLayer = other->GetLayer();
  if (otherLayer == ColliderLayer::Enemy) {
    if (!mIsInvulnerable) {
      if (!mIsBig) {
        Kill();
      } else {
        Shrink();
      }
    }
  }
  if (otherLayer == ColliderLayer::PowerUp) {
    SDL_Log("Mario colidiu horizontalmente com PowerUp!");
    Actor* powerUp = other->GetOwner();
    powerUp->SetState(ActorState::Destroy);
    Grow();
  }
}

void Player::OnVerticalCollision(const float minOverlap,
                                 AABBColliderComponent* other) {
  ColliderLayer otherLayer = other->GetLayer();

  if (otherLayer == ColliderLayer::Enemy) {
    if (minOverlap < 0.0f) {
      Actor* enemy = other->GetOwner();
      if (enemy) {
        enemy->Kill();
      }

      if (mRigidBodyComponent) {
        Vector2 vel = mRigidBodyComponent->GetVelocity();
        vel.y = mJumpSpeed;
        mRigidBodyComponent->SetVelocity(vel);
        SetOffGround();
      }
    } else {
      SDL_Log(
          "Mario atingido verticalmente por inimigo! Chamando Kill(). Overlap: "
          "%f",
          minOverlap);
      if (!mIsInvulnerable) {
        if (!mIsBig) {
          Kill();

        } else {
          Shrink();
        }
      }
    }
  } else if (otherLayer == ColliderLayer::Blocks) {
    if (minOverlap > 0.0f) {
      Actor* blockActor = other->GetOwner();

      Block* block = dynamic_cast<Block*>(blockActor);
      if (block) {
        block->StartBounce();
      }

      if (mRigidBodyComponent) {
        Vector2 vel = mRigidBodyComponent->GetVelocity();
        if (vel.y < 0.0f) {
          vel.y = 0.0f;
          mRigidBodyComponent->SetVelocity(vel);
        }
      }
    }
  } else if (otherLayer == ColliderLayer::PowerUp) {
    SDL_Log("Mario colidiu vertical com PowerUp!");
    Actor* powerUp = other->GetOwner();
    powerUp->SetState(ActorState::Destroy);
    Grow();
  }
}
void Player::Grow() {
  if (mIsBig) {
    return;
  }

  mIsBig = true;

  const std::string bigTexturePath =
      "../Assets/Sprites/SuperMario/SuperMario.png";
  const std::string bigJsonPath =
      "../Assets/Sprites/SuperMario/SuperMario.json";

  if (mDrawComponent->LoadSheet(bigTexturePath, bigJsonPath)) {
    mDrawComponent->AddAnimation("idle", {0});
    mDrawComponent->AddAnimation("run", {6, 7, 8});
    mDrawComponent->AddAnimation("jump", {2});
    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
  }
  SDL_Log("Mario cresceu! Agora ele é grande.");

  Vector2 currentScale = GetScale();
  SetScale(Vector2(currentScale.x, currentScale.y * 2.0f));
  Vector2 currentPos = GetPosition();
  SetPosition(currentPos + Vector2(0.0f, -Game::TILE_SIZE / 2.0f));
  mColliderComponent->SetSize(Vector2(Game::TILE_SIZE, Game::TILE_SIZE * 2));
}

void Player::Shrink() {
  if (!mIsBig) {
    return;
  }

  mIsBig = false;
  mIsInvulnerable = true;
  mInvulnerabilityTimer = INVULNERABILITY_DURATION;
  const std::string bigTexturePath = "../Assets/Sprites/Mario/Mario.png";
  const std::string bigJsonPath = "../Assets/Sprites/Mario/Mario.json";

  if (mDrawComponent->LoadSheet(bigTexturePath, bigJsonPath)) {
    mDrawComponent->AddAnimation("idle", std::vector<int>{1});
    mDrawComponent->AddAnimation("run", std::vector<int>{3, 4, 5});
    mDrawComponent->AddAnimation("jump", std::vector<int>{2});
    mDrawComponent->AddAnimation("dead", std::vector<int>{0});
    mDrawComponent->SetAnimation("idle");
    mDrawComponent->SetAnimFPS(10.0f);
  }
  SDL_Log("Mario agora é pequeno");

  Vector2 currentScale = GetScale();
  SetScale(Vector2(currentScale.x, currentScale.y / 2));
  Vector2 currentPos = GetPosition();
  SetPosition(currentPos + Vector2(0.0f, Game::TILE_SIZE / 2.0f));
  mColliderComponent->SetSize(Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
}

void Player::EquipWeapon(WeaponType type) {
  // (Verificação opcional para ver se já tem a arma)
  // if (GetComponent<ShotgunComponent>() != nullptr) { return; }

  switch (type) {
    case WeaponType::Pistol:
      new MainGun(this);  // (Renomeado de PistolComponent)
      break;
    case WeaponType::Shotgun:
      // new ShotgunComponent(this); // (Exemplo futuro)
      break;
  }
}