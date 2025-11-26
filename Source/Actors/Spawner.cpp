// Spawner.cpp
#include "Spawner.h"
#include "../Game.h"
#include "Goomba.h" // A classe do inimigo
#include <cstdlib>  // Para rand()
#include <algorithm> // Para std::max

Spawner::Spawner(Game* game)
    : Actor(game)
    , mSpawnTimer(0.0f)
    , mSpawnInterval(1.0f) // Começa a criar 1 inimigo por segundo
    , mDifficultyTimer(0.0f)
{
    // O Spawner é um gestor lógico, não tem sprite nem colisor.
}

void Spawner::OnUpdate(float deltaTime)
{
    // 1. Atualiza o temporizador de spawn
    mSpawnTimer -= deltaTime;
    if (mSpawnTimer <= 0.0f)
    {
        SpawnEnemy();
        mSpawnTimer = mSpawnInterval; // Reinicia o temporizador
    }

    // 2. Aumenta a dificuldade com o tempo
    // A cada 10 segundos, o spawn fica 5% mais rápido
    mDifficultyTimer += deltaTime;
    if (mDifficultyTimer > 10.0f)
    {
        mSpawnInterval = std::max(0.1f, mSpawnInterval * 0.95f); // Limite mínimo de 0.1s
        mDifficultyTimer = 0.0f;
        SDL_Log("Dificuldade Aumentada! Intervalo de Spawn: %f", mSpawnInterval);
    }
}

void Spawner::SpawnEnemy()
{
    // Pega a posição da câmara (canto superior esquerdo)
    Vector2 camPos = GetGame()->GetCameraPos();

    // Dimensões do ecrã (usar as constantes virtuais para bater com a lógica do jogo)
    float screenW = static_cast<float>(Game::VIRTUAL_WIDTH);
    float screenH = static_cast<float>(Game::VIRTUAL_HEIGHT);

    // Margem de segurança (para nascer bem fora do ecrã)
    float padding = 64.0f;

    Vector2 spawnPos;

    // Escolhe aleatoriamente um dos 4 lados:
    // 0 = Cima, 1 = Baixo, 2 = Esquerda, 3 = Direita
    int side = rand() % 4;

    switch (side)
    {
    case 0: // CIMA (Y fixo acima da câmara, X aleatório)
        spawnPos.y = camPos.y - padding;
        spawnPos.x = camPos.x + (static_cast<float>(rand()) / RAND_MAX) * screenW;
        break;

    case 1: // BAIXO (Y fixo abaixo da câmara, X aleatório)
        spawnPos.y = camPos.y + screenH + padding;
        spawnPos.x = camPos.x + (static_cast<float>(rand()) / RAND_MAX) * screenW;
        break;

    case 2: // ESQUERDA (X fixo à esquerda, Y aleatório)
        spawnPos.x = camPos.x - padding;
        spawnPos.y = camPos.y + (static_cast<float>(rand()) / RAND_MAX) * screenH;
        break;

    case 3: // DIREITA (X fixo à direita, Y aleatório)
        spawnPos.x = camPos.x + screenW + padding;
        spawnPos.y = camPos.y + (static_cast<float>(rand()) / RAND_MAX) * screenH;
        break;
    }

    // Instancia o inimigo na posição calculada
    auto enemy = new Goomba(GetGame());
    enemy->SetPosition(spawnPos);
}