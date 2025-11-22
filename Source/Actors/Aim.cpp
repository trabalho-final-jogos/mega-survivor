// Coin.cpp
#include "Aim.h"
#include "../Components/Drawing/AnimatorComponent.h"
#include "../Game.h"

Aim::Aim(Game* game, class Player* owner)
    : Actor(game), mSprite(nullptr), mOwnerPlayer(owner) {
  SetScale(Vector2(Game::TILE_SIZE, Game::TILE_SIZE));

  mSprite = new AnimatorComponent(this, "../Assets/Sprites/aim.png", "",
                                  Game::TILE_SIZE, Game::TILE_SIZE);
}

void Aim::OnUpdate(float deltaTime) {
  if (!mOwnerPlayer) {
    SDL_Log("SAIU");
    return;
  }

  // --- LÓGICA DE MOVIMENTO (MOVIDA DO PLAYER PARA CÁ) ---

  // 1. Pega a posição do mouse no MUNDO (através do Game)
  Vector2 mouseWorldPos = GetGame()->GetMousePos();

  // 2. Pega a posição do jogador (através do ponteiro mOwnerPlayer)
  Vector2 playerPos = mOwnerPlayer->GetPosition();

  // 3. Calcula o vetor de direção (do Jogador para o Mouse)
  Vector2 direction = mouseWorldPos - playerPos;

  // 4. Normaliza o vetor
  if (direction.LengthSq() > 0.0f) {
    direction.Normalize();
  } else {
    direction = Vector2(1.0f, 0.0f);  // Padrão (direita)
  }

  // 5. Calcula a nova posição da mira
  Vector2 aimerPos = playerPos + (direction * AIMER_RADIUS);
  // 6. Define a SUA PRÓPRIA posição
  SDL_Log("Vector2: x = %.2f, y = %.2f", aimerPos.x, aimerPos.y);
  SetPosition(aimerPos);
}