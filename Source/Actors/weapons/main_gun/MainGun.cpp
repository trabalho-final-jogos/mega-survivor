#include "MainGun.h"
#include "../../Actor.h"
#include "../../Player.h"
#include "../../../Game.h"
#include "Bullet.h"
#include "../../../Components/Physics/RigidBodyComponent.h"

MainGun::MainGun(Actor* owner, int updateOrder)
    : WeaponComponent(owner, updateOrder) // Chama a base
    , mProjectilePool(nullptr)
    , mAimer(nullptr)
    , mBurstVelocity(Vector2::Zero) // Inicializa a velocidade da rajada
    , mCooldownTimer(0.0f)
    , mBurstShotsLeft(0)
    , mBurstTimer(0.0f)
    , mIsFiringBurst(false)
{
    // 1. A Arma cria o seu próprio pool
    // (Não herda de Component, como corrigimos)
    mProjectilePool = new ProjectilePoolComponent();

    // 2. A Arma preenche o pool com o seu tipo de "bala"
    for (int i = 0; i < POOL_SIZE; i++)
    {
        // Cria a bala (que herda de Projectile)
        auto bullet = new Bullet(mOwner->GetGame(), PARTICLE_WIDTH, PARTICLE_HEIGHT);

        // Adiciona a bala ao pool privado desta arma
        mProjectilePool->AddObjectToPool(bullet);
    }

    // 3. Faz o "cast" do dono (Actor*) para Player*
    Player* player = static_cast<Player*>(mOwner);

    // 4. Guarda o ponteiro da mira (Aim*)
    mAimer = player->GetAim();
}

MainGun::~MainGun()
{
    // A MainGun é a dona do pool, então ela o apaga
    // (O Game é o dono das balas, o destrutor do pool DEVE estar VAZIO)
    delete mProjectilePool;
}

void MainGun::OnUpdate(float deltaTime)
{
    // 1. Atualiza o cooldown principal
    if (mCooldownTimer > 0.0f) { mCooldownTimer -= deltaTime; }

    // 2. Lógica da Rajada (Burst)
    if (mIsFiringBurst)
    {
        mBurstTimer -= deltaTime;
        if (mBurstTimer <= 0.0f)
        {
            FireShot(); // Dispara (agora usa a velocidade guardada)
            mBurstShotsLeft--;
            mBurstTimer = BURST_DELAY;

            if (mBurstShotsLeft == 0)
            {
                mIsFiringBurst = false;
                mCooldownTimer = COOLDOWN_TIME;
            }
        }
    }
    // 3. Lógica de Disparo (Automático)
    else if (mCooldownTimer <= 0.0f && !mIsFiringBurst)
    {
        // --- INICIA UMA NOVA RAJADA ---
        mIsFiringBurst = true;
        mBurstShotsLeft = BURST_COUNT;
        mBurstTimer = 0.0f; // Dispara o primeiro tiro imediatamente

        // --- CORREÇÃO DO BUG: CALCULA A VELOCIDADE *UMA VEZ* E GUARDA ---
        Player* player = static_cast<Player*>(mOwner);
        if (!mAimer || !player) { mIsFiringBurst = false; return; }

        Vector2 playerPos = player->GetPosition();
        Vector2 aimerPos = mAimer->GetPosition(); // Pega a posição da mira AGORA

        Vector2 direction = (aimerPos - playerPos);
        if (direction.LengthSq() == 0.0f) { direction = Vector2(1.0f, 0.0f); }
        direction.Normalize();

        // Pega a velocidade atual do jogador (para "herdar" o movimento)
        Vector2 playerVelocity = player->GetComponent<RigidBodyComponent>()->GetVelocity();

        // Calcula a velocidade final da bala e guarda no membro da classe
        mBurstVelocity = (direction * PROJECTILE_SPEED) + playerVelocity;
        // ----------------------------------------------------
    }
}

void MainGun::FireShot()
{
    // Esta função agora é "burra". Ela não calcula mais a direção.
    // Ela apenas usa a direção que foi guardada em mBurstVelocity.

    // Pega a posição de disparo (o jogador pode ter-se movido desde o início da rajada)
    Vector2 playerPos = mOwner->GetPosition();

    Projectile* p = mProjectilePool->GetDeadObject();
    if (p)
    {
        // "Acorda" a bala na posição atual do jogador
        p->Awake(mOwner, playerPos, mOwner->GetRotation(), PROJECTILE_LIFETIME);

        // --- USA A VELOCIDADE GUARDADA ---
        p->GetComponent<RigidBodyComponent>()->SetVelocity(mBurstVelocity);
    }
}