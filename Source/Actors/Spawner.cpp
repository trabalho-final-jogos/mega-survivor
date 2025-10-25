//
// Created by Lucas N. Ferreira on 30/09/23.
//

#include "Spawner.h"
#include "../Game.h"
#include "Mario.h"
#include "Goomba.h"

Spawner::Spawner(Game* game, float spawnDistance)
        :Actor(game)
        ,mSpawnDistance(spawnDistance)
{

}

void Spawner::OnUpdate(float deltaTime)
{
        auto mario = mGame->GetPlayer();
        if (!mario) return;

        float distance = Vector2::Distance(mario->GetPosition(), GetPosition());

        Vector2 marioPos = mario->GetPosition();
        Vector2 spawnerPos = GetPosition();

        if (distance < mSpawnDistance)
        {

                auto goomba = new Goomba(mGame);
                goomba->SetPosition(GetPosition());
                this->SetState(ActorState::Destroy); // remove o spawner após criar o inimigo


        }
}