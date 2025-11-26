//
// Created by filipe on 11/4/25.
//

#include "AuraWeapon.h"
#include "../../../Components/ProjectilePoolComponent.h"
#include "AuraActor.h"
#include "../../../Game.h"
#include "../../../Components/Physics/AABBColliderComponent.h"
#include "../../../Components/Drawing/AnimatorComponent.h" // <-- Usa um Sprite, não um Animator
#include "../../../Components/Physics/RigidBodyComponent.h"
#include "../../Player.h" // Necessário para pegar o Ator do Player e a Mira
#include "../../Aim.h"  // Necessário para pegar a posição da Mira
#include "../../../Math.h" // Para ToRadians e Vector2::Perpendicular

AuraWeapon::AuraWeapon(Actor* owner, int updateOrder)
    // 1. Chama o construtor da base (WeaponComponent)
    //    Identifica-se como WeaponType::Aura
    : WeaponComponent(owner, WeaponType::Aura, updateOrder)
    , mAura(nullptr)
{
    // 2. O "Cérebro" (AuraWeapon) cria o "Corpo" (AuraActor)
    //    O AuraActor é um Ator normal e irá registar-se
    //    automaticamente no loop 'mActors' do Game.
    mAura = new AuraActor(owner->GetGame(), static_cast<Player*>(owner));
}

AuraWeapon::~AuraWeapon()
{
    // 3. Quando a arma (cérebro) é destruída (ex: Player morre),
    //    ela manda o seu "corpo" (AuraActor) se destruir também.
    if (mAura)
    {
        mAura->SetState(ActorState::Destroy);
    }
}

void AuraWeapon::OnUpdate(float deltaTime)
{
    // A lógica de dano, timer e seguir o jogador está
    // toda dentro de AuraActor::OnUpdate().
    // Portanto, o update da arma "cérebro" pode ficar vazio.
}

void AuraWeapon::LevelUp() {
    // 4. Repassa o comando de LevelUp para o "corpo"
    if (mAura)
    {
        mAura->LevelUp();
    }

    // 5. Atualiza o nível do próprio "cérebro" (para referência futura)
    mLevel++;
}