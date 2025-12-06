// Spawner.cpp
#include "Spawner.h"
#include "../Game.h"
#include "Player.h"
#include "enemies/Bat.h"
#include "enemies/Enemy1.h"

#include <algorithm>  // Para std::max
#include <cstdlib>    // Para rand()

Spawner::Spawner(Game* game)
    : Actor(game),
      mSpawnTimer(0.0f),
      mCurrentWaveIndex(0),
      mCurrentWaveSide(-1)  // -1 = Aleatório (Padrão)
{
  SetupWaves();
}

void Spawner::SetupWaves() {
  // --- CONFIGURAÇÃO DO GAME DESIGN ---
  // { Inicio, Fim, Intervalo, Tipo, Vida, Velocidade }
  //mWaves.push_back({0.0f, 0.5f, 0.5f, EnemyType::Metall, 10.0f, 40.0f, 5.0f});

  mWaves.push_back({0.0f, 30.0f, 0.8f, EnemyType::Metall, 10.0f, 40.0f, 1.0f});

  //mWaves.push_back({15.0f, 30.0f, 1.0f, EnemyType::Metall, 20.0f, 60.0f});

  // Onda 3 (30-45s): ENXAME DE MORCEGOS! (Rápidos, morrem com 1 hit, muitos)
  mWaves.push_back({30.0f, 31.0f, 0.1f, EnemyType::Bat, 1.0f, 250.0f, 1.0f});

  mWaves.push_back({31.0f, 60.0f, 0.7f, EnemyType::Metall, 15.0f, 40.0f, 1.0f});
  mWaves.push_back({60.0f, 60.5f, 0.49f, EnemyType::Metall, 100.0f, 55.0f, 5.0f});

  mWaves.push_back({60.6f, 61.6f, 0.1f, EnemyType::Bat, 1.0f, 250.0f, 1.0f});
  mWaves.push_back({61.7f, 220.0f, 0.65f, EnemyType::Metall, 30.0f, 65.0f, 1.0f});


}

void Spawner::OnUpdate(float deltaTime) {
  float gameTime = GetGame()->GetClockTime();

  // --- 1. GERENCIAMENTO DE ONDAS ---
  if (mCurrentWaveIndex < mWaves.size()) {
    const SpawnWave& currentWave = mWaves[mCurrentWaveIndex];

    // Se o tempo passou do fim desta onda...
    if (gameTime >= currentWave.endTime) {
      mCurrentWaveIndex++;  // Avança

      // Verifica a NOVA onda para configurar comportamentos especiais (Enxame)
      if (mCurrentWaveIndex < mWaves.size()) {
        const SpawnWave& nextWave = mWaves[mCurrentWaveIndex];

        // Se for Morcego, TRAVA um lado aleatório para criar o efeito de
        // "travessia"
        if (nextWave.type == EnemyType::Bat) {
          mCurrentWaveSide = rand() % 4;
          SDL_Log("!!! ENXAME DE MORCEGOS VINDO DO LADO %d !!!",
                  mCurrentWaveSide);
        } else {
          mCurrentWaveSide = -1;  // Volta ao spawn aleatório normal
        }
      }
    }
  }

  // --- 2. DETERMINAR INTERVALO ---
  float interval = 1.0f;
  if (mCurrentWaveIndex < mWaves.size()) {
    interval = mWaves[mCurrentWaveIndex].spawnInterval;
  } else if (!mWaves.empty()) {
    interval = mWaves.back().spawnInterval;
  }

  // --- 3. SPAWN ---
  mSpawnTimer -= deltaTime;
  if (mSpawnTimer <= 0.0f) {
    SpawnEnemy();
    mSpawnTimer = interval;
  }
}

void Spawner::SpawnEnemy() {
  // Pega dados da onda atual
  const SpawnWave* wave = nullptr;
  if (mCurrentWaveIndex < mWaves.size())
    wave = &mWaves[mCurrentWaveIndex];
  else if (!mWaves.empty())
    wave = &mWaves.back();
  else
    return;

  // Dados da tela e câmera
  Vector2 camPos = GetGame()->GetCameraPos();
  float screenW = static_cast<float>(Game::VIRTUAL_WIDTH);
  float screenH = static_cast<float>(Game::VIRTUAL_HEIGHT);
  float padding = 64.0f;  // Distância fora da tela

  // --- DECISÃO DO LADO ---
  int side = (mCurrentWaveSide != -1) ? mCurrentWaveSide : (rand() % 4);

  Vector2 spawnPos;

  // Cálculo da posição na borda
  switch (side) {
    case 0:  // Cima
      spawnPos.y = camPos.y - padding;
      spawnPos.x = camPos.x + (static_cast<float>(rand()) / RAND_MAX) * screenW;
      break;
    case 1:  // Baixo
      spawnPos.y = camPos.y + screenH + padding;
      spawnPos.x = camPos.x + (static_cast<float>(rand()) / RAND_MAX) * screenW;
      break;
    case 2:  // Esquerda
      spawnPos.x = camPos.x - padding;
      spawnPos.y = camPos.y + (static_cast<float>(rand()) / RAND_MAX) * screenH;
      break;
    case 3:  // Direita
      spawnPos.x = camPos.x + screenW + padding;
      spawnPos.y = camPos.y + (static_cast<float>(rand()) / RAND_MAX) * screenH;
      break;
  }

  // --- INSTANCIAÇÃO DO INIMIGO ---

  if (wave->type == EnemyType::Metall) {
    auto mettal = new Enemy1(GetGame());
    mettal->SetPosition(spawnPos);
    // Injeta Stats (Vida, Velocidade)
    mettal->SetStats(wave->enemyHealth, wave->enemySpeed);
    mettal->SetScale(Vector2(32.0f*wave->enemySize, 32.0f * wave->enemySize));
  } else if (wave->type == EnemyType::Bat) {
    auto bat = new Bat(GetGame());
    bat->SetPosition(spawnPos);
    // Injeta Stats (Vida)
    // (Nota: Bat usa velocidade no SetFixedVelocity, mas podemos setar vida
    // aqui) bat->SetStats(wave->enemyHealth);

    // --- LÓGICA DE MIRA DO MORCEGO (LINHA RETA) ---
    // Calcula direção para atravessar a tela em direção à área do player
    Vector2 playerPos = Vector2::Zero;
    if (GetGame()->GetPlayer()) {
      playerPos = GetGame()->GetPlayer()->GetPosition();
    }

    // Adiciona uma variação aleatória para não parecerem robôs perfeitos
    float randomOffset =
        (static_cast<float>(rand()) / RAND_MAX) * 100.0f - 50.0f;

    Vector2 target;
    if (side == 0 || side == 1)
      target = Vector2(spawnPos.x + randomOffset,
                       playerPos.y);  // Cruza verticalmente
    else
      target = Vector2(playerPos.x,
                       spawnPos.y + randomOffset);  // Cruza horizontalmente

    Vector2 direction = playerPos - spawnPos;  // Mira no player atual
    if (direction.LengthSq() > 0)
      direction.Normalize();

    // Define velocidade fixa
    bat->SetFixedVelocity(direction * wave->enemySpeed);
  }
}