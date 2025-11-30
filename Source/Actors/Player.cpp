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
#include "XPGem.h"
#include "weapons/WeaponType.h"
#include "weapons/aura/AuraWeapon.h"
#include "weapons/boomerang/BoomerangGun.h"
#include "weapons/ice_gun/IceGun.h"
#include "weapons/laser_beam/LaserGun.h"
#include "weapons/main_gun/MainGun.h"
#include "weapons/saw_blade/SawGun.h"

std::string getPlayerTexturePath(PlayerChar character) {
    switch (character) {
        case PlayerChar::MEGAMAN:
            return "../Assets/Sprites/Megaman/player.png";
          case PlayerChar::PROTOMAN:
            return "../Assets/Sprites/Protoman/player.png";
          case PlayerChar::BASS:
            return "../Assets/Sprites/Bass/player.png";
          default:
            return "../Assets/Sprites/Megaman/player.png";
    }
}

std::string getPlayerDataPath(PlayerChar character) {
    switch (character) {
        case PlayerChar::MEGAMAN:
            return "../Assets/Sprites/Megaman/player.json";
        case PlayerChar::PROTOMAN:
            return "../Assets/Sprites/Protoman/player.json";
        case PlayerChar::BASS:
            return "../Assets/Sprites/Bass/player.json";
        default:
            return "../Assets/Sprites/Megaman/player.json";
    }
}

Player::Player(Game* game, PlayerChar pchar, const float forwardSpeed, const float jumpSpeed)
    : Actor(game),
      mIsRunning(false),
      mIsDead(false),
      mForwardSpeed(forwardSpeed),
      mJumpSpeed(jumpSpeed),
      mIsBig(false),
      mIsInvulnerable(false),
      mInvulnerabilityTimer(0.0f),
      mAimer(nullptr) {
  SetScale(Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
  mDrawComponent = new AnimatorComponent(
      this, getPlayerTexturePath(pchar),
      getPlayerDataPath(pchar)
      , Game::TILE_SIZE, Game::TILE_SIZE);
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
}

void Player::OnProcessInput(const uint8_t* state) {
  if (!mRigidBodyComponent) {
    return;
  }

  mIsRunning = false;
  Vector2 velocity = Vector2::Zero;  // Começa com velocidade zero

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
  }
  if (state[SDL_SCANCODE_A]) {
    velocity.x = -mForwardSpeed;
    mIsRunning = true;
  }

  if (velocity.LengthSq() > mForwardSpeed * mForwardSpeed) {
    velocity.Normalize();
    velocity *= mForwardSpeed;
  }

  mRigidBodyComponent->SetVelocity(velocity);

  // Input de teste para habilitar e desabilitar as armas
  //  ---- Detecção de um toque por tecla (flags estáticas) ----
  static bool keyDown[7] = {false};
  // Usaremos índices 1–6, ignorando o 0.

  // ---- Tecla 1: Alterna MainGun ----
  if (state[SDL_SCANCODE_1]) {
    if (!keyDown[1]) {
      keyDown[1] = true;

      MainGun* gun = GetComponent<MainGun>();
      if (gun && gun->IsEnabled()) {
        UnequipWeapon(WeaponType::MainGun);
      } else {
        EquipWeapon(WeaponType::MainGun);
      }
    }
  } else
    keyDown[1] = false;

  // ---- Tecla 2: Alterna IceGun ----
  if (state[SDL_SCANCODE_2]) {
    if (!keyDown[2]) {
      keyDown[2] = true;

      IceGun* gun = GetComponent<IceGun>();
      if (gun && gun->IsEnabled()) {
        UnequipWeapon(WeaponType::IceGun);
      } else {
        EquipWeapon(WeaponType::IceGun);
      }
    }
  } else
    keyDown[2] = false;

  // ---- Tecla 3: Alterna BoomerangGun ----
  if (state[SDL_SCANCODE_3]) {
    if (!keyDown[3]) {
      keyDown[3] = true;

      BoomerangGun* gun = GetComponent<BoomerangGun>();
      if (gun && gun->IsEnabled()) {
        UnequipWeapon(WeaponType::BoomerangGun);
      } else {
        EquipWeapon(WeaponType::BoomerangGun);
      }
    }
  } else
    keyDown[3] = false;

  // ---- Tecla 4: Alterna SawGun ----
  if (state[SDL_SCANCODE_4]) {
    if (!keyDown[4]) {
      keyDown[4] = true;

      SawGun* gun = GetComponent<SawGun>();
      if (gun && gun->IsEnabled()) {
        UnequipWeapon(WeaponType::SawGun);
      } else {
        EquipWeapon(WeaponType::SawGun);
      }
    }
  } else
    keyDown[4] = false;

  // ---- Tecla 5: Alterna Aura ----
  if (state[SDL_SCANCODE_5]) {
    if (!keyDown[5]) {
      keyDown[5] = true;

      AuraWeapon* gun = GetComponent<AuraWeapon>();
      if (gun && gun->IsEnabled()) {
        UnequipWeapon(WeaponType::Aura);
      } else {
        EquipWeapon(WeaponType::Aura);
      }
    }
  } else
    keyDown[5] = false;

  // ---- Tecla 6: Alterna LaserGun ----
  if (state[SDL_SCANCODE_6]) {
    if (!keyDown[6]) {
      keyDown[6] = true;

      AuraWeapon* gun = GetComponent<AuraWeapon>();
      if (gun && gun->IsEnabled()) {
        UnequipWeapon(WeaponType::LaserGun);
      } else {
        EquipWeapon(WeaponType::LaserGun);
      }
    }
  } else {
    keyDown[6] = false;
  }
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
    }
  }
  if (mRigidBodyComponent) {
    Vector2 velocity = mRigidBodyComponent->GetVelocity();
  }

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
  OnVerticalCollision(minOverlap, other);
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
    }
  } else if (otherLayer == ColliderLayer::XP) {
    Actor* xpOwner = other->GetOwner();
    XPGem* xpGem = dynamic_cast<XPGem*>(xpOwner);

    if (xpGem) {
      AddXP(xpGem->GetXPValue());
      xpGem->SetState(ActorState::Destroy);
    }
  }
}

void Player::EquipWeapon(WeaponType type) {
  Component* compToEquip = nullptr;
  switch (type) {
    case WeaponType::MainGun:
      compToEquip = GetComponent<MainGun>();
      break;
    case WeaponType::IceGun:
      compToEquip = GetComponent<IceGun>();
      break;
    case WeaponType::BoomerangGun:
      compToEquip = GetComponent<BoomerangGun>();
      break;
    case WeaponType::SawGun:
      compToEquip = GetComponent<SawGun>();
      break;
    case WeaponType::Aura:
      compToEquip = GetComponent<AuraWeapon>();
      break;
    case WeaponType::LaserGun:
      compToEquip = GetComponent<LaserGun>();
      break;
  }

  // 2. Se a arma já existe...
  if (compToEquip != nullptr) {
    // ...apenas a reativa.
    compToEquip->SetEnabled(true);
    SDL_Log("DEBUG: Reativando arma tipo: %d", (int)type);
  }
  // 3. Se a arma não existe, cria-a (Nível 1)
  else {
    SDL_Log("DEBUG: Criando nova arma tipo: %d", (int)type);
    switch (type) {
      case WeaponType::MainGun:
        new MainGun(this);
        break;
      case WeaponType::IceGun:
        new IceGun(this);
        break;
      case WeaponType::BoomerangGun:
        new BoomerangGun(this);
        break;
      case WeaponType::SawGun:
        new SawGun(this);
        break;
      case WeaponType::Aura:
        new AuraWeapon(this);
        break;
      case WeaponType::LaserGun:
        new LaserGun(this);
        break;
    }
  }
}

void Player::UnequipWeapon(WeaponType type) {
  Component* compToUnequip = nullptr;

  // Encontra o componente da arma pelo tipo
  switch (type) {
    case WeaponType::MainGun:
      compToUnequip = GetComponent<MainGun>();
      break;
    case WeaponType::IceGun:
      compToUnequip = GetComponent<IceGun>();
      break;
    case WeaponType::BoomerangGun:
      compToUnequip = GetComponent<BoomerangGun>();
      break;
    case WeaponType::SawGun:
      compToUnequip = GetComponent<SawGun>();
      break;
    case WeaponType::Aura:
      compToUnequip = GetComponent<AuraWeapon>();
      break;
    case WeaponType::LaserGun:
      compToUnequip = GetComponent<LaserGun>();
      break;
  }

  // Se encontramos a arma...
  if (compToUnequip != nullptr) {
    SDL_Log("DEBUG: Desativando arma tipo: %d", (int)type);
    // Apenas a desativa. Ela continua a existir.
    compToUnequip->SetEnabled(false);
  }
}

void Player::AddXP(uint32_t amount) {
  mCurrentXp += amount;
  SDL_Log("Added %d XP. Total XP: %d", amount, mCurrentXp);
  // Logic for leveling up could go here

  while (mCurrentXp >= GetMaxXP()) {
    mCurrentXp -= GetMaxXP();
    mCurrentLvl++;
    SDL_Log("Level Up! New Level: %d", mCurrentLvl);
  }
}

uint32_t Player::GetMaxXP() const {
  return 100 * (mCurrentLvl + 1);
}
