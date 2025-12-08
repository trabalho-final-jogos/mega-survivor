// Spawner.cpp
#include "Spawner.h"
#include "../Game.h"
#include "Player.h"
#include "enemies/Bat.h"
#include "enemies/Enemy1.h"
#include "enemies/EnemyGround.h"
#include "enemies/EnemyFly.h"
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
 mWaves.clear();

    // Configuração dos Morcegos (para facilitar ajustes)
    float batDuration = 2.3f;       // A horda dura 5 segundos
    float batInterval = 0.1f;       // Muito rápido (enxame)
    float batSpeed = 250.0f;        // Muito rápido
    float batHealth = 1.0f;         // Morre com 1 hit

    // =================================================================================
    // FASE 1: METALLS (00:00 até 03:20 / 200s)
    // Objetivo: Ficam mais rápidos e fortes a cada intervalo
    // =================================================================================

    mWaves.push_back({ 0.0f, 15.0f, 1.5f, EnemyType::Metall, 10.0f, 40.0f , 1.0f});

    // --- BAT WAVE 1 (00:15) ---
    mWaves.push_back({ 15.0f, 15.0f + batDuration, batInterval, EnemyType::Bat, batHealth, batSpeed, 1.0f });

    // 00:20 -> 01:15 (Metall Normal) - Retoma após morcegos
    mWaves.push_back({ 20.0f, 75.0f, 1.0f, EnemyType::Metall, 20.0f, 50.0f, 1.0f });

    // --- BAT WAVE 2 (01:15) --- (60s após a primeira)
    mWaves.push_back({ 75.0f, 75.0f + batDuration, batInterval, EnemyType::Bat, batHealth, batSpeed, 1.0f });

    // 01:20 -> 02:15 (Metall Rápido)
    mWaves.push_back({ 80.0f, 135.0f, 0.8f, EnemyType::Metall, 40.0f, 70.0f, 1.0f });

    // --- BAT WAVE 3 (02:15) ---
    mWaves.push_back({ 135.0f, 135.0f + batDuration, batInterval, EnemyType::Bat, batHealth, batSpeed, 1.0f });

    // 02:20 -> 03:15 (Metall Tank/Forte)
    mWaves.push_back({ 140.0f, 195.0f, 0.5f, EnemyType::Metall, 80.0f, 50.0f, 1.0f });

    // --- BAT WAVE 4 (03:15) ---
    mWaves.push_back({ 195.0f, 200.0f, batInterval, EnemyType::Bat, batHealth, batSpeed, 1.0f });
    mWaves.push_back({ 200.0f, 200.5f, 0.4f, EnemyType::Metall, 205.0f, 50.0f, 5.0f });

    // =================================================================================
    // FASE 2: GROUND (03:20 até 06:40 / 200s a 400s)
    // Objetivo: Inimigos de chão, provavelmente mais resistentes
    // =================================================================================

    // 03:20 -> 04:15 (Ground Normal)
    mWaves.push_back({ 200.6f, 255.0f, 1.5f, EnemyType::Ground, 50.0f, 30.0f, 1.0f });

    // --- BAT WAVE 5 (04:15) ---
    mWaves.push_back({ 255.0f, 255.0f + batDuration, batInterval, EnemyType::Bat, batHealth, batSpeed, 1.0f });

    // 04:20 -> 05:15 (Ground Rápido)
    mWaves.push_back({ 260.0f, 315.0f, 1.0f, EnemyType::Ground, 80.0f, 50.0f, 1.0f });

    // --- BAT WAVE 6 (05:15) ---
    mWaves.push_back({ 315.0f, 315.0f + batDuration, batInterval, EnemyType::Bat, batHealth, batSpeed  , 1.0f});

    // 05:20 -> 06:15 (Ground Horda)
    mWaves.push_back({ 320.0f, 375.0f, 0.5f, EnemyType::Ground, 100.0f, 60.0f, 1.0f });

    mWaves.push_back({ 375.0f, 375.5f, 0.4f, EnemyType::Ground, 230.0f, 60.0f, 5.0f });



    // =================================================================================
    // FASE 3: FLY (06:20 até 10:00 / 400s a 600s)
    // Objetivo: Inimigos voadores até o fim do jogo
    // =================================================================================

    // 06:20 -> 07:15 (Fly Inicial)
    // (Use EnemyType::Fly se tiver, ou Metall muito rápido como placeholder)
    mWaves.push_back({ 376.0f, 435.0f, 1.0f, EnemyType::Fly, 30.0f, 100.0f, 1.0f});

    // --- BAT WAVE 8 (07:15) ---
    mWaves.push_back({ 435.0f, 435.0f + batDuration, batInterval, EnemyType::Bat, batHealth, batSpeed,1.0f });

    // 07:20 -> 08:15 (Fly Médio)
    mWaves.push_back({ 440.0f, 495.0f, 0.8f, EnemyType::Fly, 50.0f, 110.0f, 1.0f });

    // --- BAT WAVE 9 (08:15) ---
    mWaves.push_back({ 495.0f, 495.0f + batDuration, batInterval, EnemyType::Bat, batHealth, batSpeed, 1.0f });

    // 08:20 -> 09:15 (Fly Difícil)
    mWaves.push_back({ 500.0f, 555.0f, 0.5f, EnemyType::Fly, 80.0f, 120.0f, 1.0f });

    // --- BAT WAVE 10 (09:15) ---
    mWaves.push_back({ 555.0f, 560.0f, batInterval, EnemyType::Bat, batHealth, batSpeed, 1.0f });

    mWaves.push_back({ 560.0f, 560.5f, 0.4f, EnemyType::Fly, 280.0f, 130.0f, 5.0f });

    // 09:20 -> FIM (10:00+) (Fly Insano / Final)
    mWaves.push_back({ 560.0f, 9999.0f, 0.2f, EnemyType::Random, 100.0f, 130.0f, 1.0f });
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
  }else if (wave->type == EnemyType::Ground) {

    auto ground = new EnemyGround(GetGame());
    ground->SetPosition(spawnPos);
    // Injeta Stats (Vida, Velocidade)
    ground->SetStats(wave->enemyHealth, wave->enemySpeed);
    ground->SetScale(Vector2(32.0f*wave->enemySize, 32.0f * wave->enemySize));
  }if (wave->type == EnemyType::Fly) {

    auto fly = new EnemyFly(GetGame());
    fly->SetPosition(spawnPos);
    // Injeta Stats (Vida, Velocidade)
    fly->SetStats(wave->enemyHealth, wave->enemySpeed);
    fly->SetScale(Vector2(32.0f*wave->enemySize, 32.0f * wave->enemySize));
  }else if (wave->type == EnemyType::Random) {
    int r = rand() % 3;

    switch (r) {
      case 0: {
        auto spawn_mettal = new Enemy1(GetGame());
        spawn_mettal->SetPosition(spawnPos);
        spawn_mettal->SetStats(wave->enemyHealth, wave->enemySpeed);
      }break;
      case 1: {
        auto spawn_ground = new EnemyGround(GetGame());
        spawn_ground->SetPosition(spawnPos);
        spawn_ground->SetStats(wave->enemyHealth, wave->enemySpeed);
      }break;
      case 2: {
        auto spawn_fly = new EnemyFly(GetGame());
        spawn_fly->SetPosition(spawnPos);
        spawn_fly->SetStats(wave->enemyHealth, wave->enemySpeed);
      }
    }

  }
}
