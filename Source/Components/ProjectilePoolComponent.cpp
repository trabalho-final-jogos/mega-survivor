//
// Created by filipe on 11/1/25.
//

#include "ProjectilePoolComponent.h"

// ProjectilePoolComponent.cpp
#include "ProjectilePoolComponent.h"
#include "../Actors/weapons/Projectile.h" // Inclui a classe base

ProjectilePoolComponent::ProjectilePoolComponent()
{
            // O construtor é VAZIO.
    // A Arma (ex: MainGun) será responsável por criar
    // e adicionar os projéteis a este pool.
}

ProjectilePoolComponent::~ProjectilePoolComponent()
{

    mPool.clear();
}

// Adiciona um projétil (ex: um Bullet) ao pool
void ProjectilePoolComponent::AddObjectToPool(Projectile* projectile)
{
    mPool.push_back(projectile);
}

// Encontra o primeiro projétil inativo disponível no pool
Projectile* ProjectilePoolComponent::GetDeadObject()
{
    for (auto p : mPool)
            {
        if (p->IsDead())
                        {
            return p; // Retorna o primeiro projétil morto
                        }
            }
    return nullptr; // Nenhum projétil disponível (pool está todo em uso)
}