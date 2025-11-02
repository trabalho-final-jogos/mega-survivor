// ProjectilePoolComponent.h
#pragma once
#include "Component.h"
#include <vector>

// Forward declaration da nossa nova classe base
class Projectile;

class ProjectilePoolComponent
{
public:
    ProjectilePoolComponent();
    ~ProjectilePoolComponent(); // Destrutor para limpar o pool

    // Adiciona um projétil pré-criado ao pool
    void AddObjectToPool(Projectile* projectile);

    // Pega um projétil "morto" (inativo) do pool
    Projectile* GetDeadObject();

private:
          std::vector<Projectile*> mPool; // O pool agora armazena Projéteis
};