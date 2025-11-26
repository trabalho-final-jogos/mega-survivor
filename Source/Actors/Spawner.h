// Spawner.h
#pragma once
#include "Actor.h"
#include "../Math.h"

class Spawner : public Actor
{
public:
    Spawner(class Game* game);

    void OnUpdate(float deltaTime) override;

private:
    void SpawnEnemy();

    // Temporizador para controlar a frequência de criação
    float mSpawnTimer;
    float mSpawnInterval; // Tempo entre inimigos (ex: 1.0s)

    // Dificuldade: diminui o intervalo com o tempo
    float mDifficultyTimer;
};