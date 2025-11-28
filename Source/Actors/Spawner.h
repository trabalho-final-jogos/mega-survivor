// Spawner.h
#pragma once
#include "Actor.h"
#include "enemies/EnemyType.h"
#include "../Math.h"

struct SpawnWave
{
    float startTime;      // Quando começa
    float endTime;        // Quando termina
    float spawnInterval;  // Tempo entre inimigos
    EnemyType type;       // Tipo de inimigo
    float enemyHealth;    // Status: Vida
    float enemySpeed;     // Status: Velocidade
};

class Spawner : public Actor
{
public:
    Spawner(class Game* game);

    void OnUpdate(float deltaTime) override;

private:
    void SetupWaves();  // Configura a lista de ondas
    void SpawnEnemy();  // Cria o inimigo na borda da tela

    std::vector<SpawnWave> mWaves;
    size_t mCurrentWaveIndex;

    float mSpawnTimer;

    int mCurrentWaveSide;
};