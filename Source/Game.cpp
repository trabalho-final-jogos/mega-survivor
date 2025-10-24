// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include <algorithm>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "CSV.h"
#include "Game.h"
#include "Components/Drawing/DrawComponent.h"
#include "Components/Physics/RigidBodyComponent.h"
#include "Random.h"
#include "Actors/Actor.h"
#include "Actors/Block.h"
#include "Actors/Goomba.h"
#include "Actors/Spawner.h"
#include "Actors/Mario.h"
#include "Actors/QuestionBlock.h"

Game::Game()
        :mWindow(nullptr)
        ,mRenderer(nullptr)
        ,mTicksCount(0)
        ,mIsRunning(true)
        ,mIsDebugging(false)
        ,mUpdatingActors(false)
        ,mCameraPos(Vector2::Zero)
        ,mMario(nullptr)
        ,mLevelData(nullptr)
{

}

bool Game::Initialize()
{
    Random::Init();

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("TP3: Super Mario Bros", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = new Renderer(mWindow);
    mRenderer->Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Init all game actors
    InitializeActors();

    mTicksCount = SDL_GetTicks();
    mIsDebugging = true;
    return true;
}

void Game::InitializeActors()
{
    //mRenderer->DrawDebugRect();
    mLevelData = LoadLevel("../Assets/Levels/Level1-1/level1-1.csv", LEVEL_WIDTH, LEVEL_HEIGHT);
    BuildLevel(mLevelData, LEVEL_WIDTH,LEVEL_HEIGHT);
    UpdateCamera();
    // 2. Verifica se o carregamento foi bem-sucedido antes de tentar imprimir
    if (mLevelData)
    {
        // Mensagem inicial para facilitar a localização no log
        SDL_Log("--- Imprimindo Matriz Completa do Nível (%d x %d) ---", LEVEL_HEIGHT, LEVEL_WIDTH);

        // Loop pelas LINHAS (índice 'i')
        for (int i = 0; i < LEVEL_HEIGHT; ++i)
        {
            // std::stringstream é uma forma eficiente de construir uma string
            std::stringstream ss;

            // Loop pelas COLUNAS (índice 'j')
            for (int j = 0; j < LEVEL_WIDTH; ++j)
            {
                // Adiciona o número (ID do tile) e uma vírgula à string da linha
                ss << mLevelData[i][j] << ",";
            }

            // Imprime a string da linha inteira de uma só vez
            SDL_Log("%s", ss.str().c_str());
        }

        // Mensagem final para indicar que a impressão terminou
        SDL_Log("--- Fim da Matriz do Nível ---");
    }
    else
    {
        SDL_Log("ERRO: mLevelData é nulo. A matriz do nível não foi carregada e não pode ser impressa.");
    }

    // ... o resto do seu código de inicialização (criar Mario, etc.) ...
}

int **Game::LoadLevel(const std::string& fileName, int width, int height)
{
    int** levelData = new int*[height];
    for (int i = 0; i < height; ++i)
    {
        levelData[i] = new int[width];
    }

    // 2. Abre o arquivo para leitura
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        SDL_Log("Erro: Nao foi possivel abrir o arquivo de nivel '%s'", fileName.c_str());
        // Libera a memória alocada antes de retornar com falha
        for (int i = 0; i < height; ++i) { delete[] levelData[i]; }
        delete[] levelData;
        return nullptr;
    }

    // 3. Lê o arquivo linha por linha
    std::string line;
    int currentRow = 0;
    while (std::getline(file, line))
    {
        // Se já lemos todas as linhas esperadas, paramos
        if (currentRow >= height) {
            break;
        }

        // 4. Usa o seu helper para dividir a linha em um vetor de inteiros
        std::vector<int> parsedRow = CSVHelper::Split(line);

        // Verifica se a linha tem a largura correta
        if (parsedRow.size() != width)
        {
            SDL_Log("Erro na linha %d do nivel: largura esperada %d, mas encontrou %zu", currentRow, width, parsedRow.size());
            // Lida com o erro (pode ser melhorado, mas por enquanto vamos pular a linha)
            continue;
        }

        // 5. Copia os dados do vetor para a nossa matriz C-style
        for (int col = 0; col < width; ++col)
        {
            levelData[currentRow][col] = parsedRow[col];
        }

        currentRow++;
    }

    file.close();

    SDL_Log("Nivel '%s' carregado com sucesso.", fileName.c_str());
    return levelData;
}

void Game::BuildLevel(int** levelData, int width, int height)
{
    if (!levelData)
    {
        SDL_Log("ERRO: Tentativa de construir nível sem dados carregados (mLevelData é nulo).");
        return;
    }

    // Percorre cada célula da matriz do nível.
    // 'i' representa a linha (coordenada Y da grade).
    for (int i = 0; i < height; ++i)
    {
        // 'j' representa a coluna (coordenada X da grade).
        for (int j = 0; j < width; ++j)
        {
            int tileID = levelData[i][j];

            // Pula as células vazias (assumindo que -1 ou outro número negativo seja vazio).
            // Se 0 for um bloco, essa verificação pode ser removida ou alterada.
            if (tileID < 0)
            {
                continue;
            }

            // Calcula a posição do CENTRO do tile no mundo do jogo.
            // Isso converte as coordenadas da grade (i, j) em coordenadas de pixel (x, y).
            Vector2 pos(
                static_cast<float>(j * TILE_SIZE) + (TILE_SIZE / 2.0f),
                static_cast<float>(i * TILE_SIZE) + (TILE_SIZE / 2.0f)
            );

            // Cria o ator apropriado com base no ID do tile.
            switch (tileID)
            {
                // --- PERSONAGEM ---
                case 16: // ID para a posição inicial do Mario
                    SDL_Log("criou o mario");
                    mMario = new Mario(this);
                    mMario->SetPosition(pos);
                    break;

                case 10: {
                    auto spawner = new Spawner(this, 400.f);
                    spawner->SetPosition(pos);
                }
                    break;
                // --- BLOCOS (IDs 0 a 15) ---
                case 0: { auto b = new Block(this, "../Assets/Sprites/Blocks/BlockA.png"); b->SetPosition(pos); } break;
                case 1: { auto b = new QuestionBlock(this, "../Assets/Sprites/Blocks/BlockC.png", "Block ?"); b->SetPosition(pos); } break;
                case 2: { auto b = new Block(this, "../Assets/Sprites/Blocks/BlockF.png"); b->SetPosition(pos); } break;
                case 3: { auto b = new Block(this, "../Assets/Sprites/Collectables/Coin.png"); b->SetPosition(pos); } break;
                case 4: { auto b = new Block(this, "../Assets/Sprites/Blocks/BlockB.png", "Block B"); b->SetPosition(pos); } break;
                case 5: { auto b = new Block(this, "../Assets/Sprites/Blocks/BlockE.png"); b->SetPosition(pos); } break;
                case 6: { auto b = new Block(this, "../Assets/Sprites/Blocks/BlockI.png"); b->SetPosition(pos); } break;
                //case 7: { auto b = new Block(this, "../Assets/Sprites/Blocks/BlockH.png"); b->SetPosition(pos); } break;
                case 8: { auto b = new Block(this, "../Assets/Sprites/Blocks/BlockD.png"); b->SetPosition(pos); } break;
                case 9: { auto b = new Block(this, "../Assets/Sprites/Blocks/BlockH.png"); b->SetPosition(pos); } break;

                //case 11: { auto b = new Block(this, "../Assets/Sprites/Blocks/BlockL.png"); b->SetPosition(pos); } break;
                case 12: { auto b = new Block(this, "../Assets/Sprites/Blocks/BlockG.png"); b->SetPosition(pos); } break;
                case 13: { auto b = new QuestionBlock(this, "../Assets/Sprites/Blocks/BlockC.png", "Block Mushroom"); b->SetPosition(pos); } break;

                // Adicione aqui outros cases para canos, inimigos, etc.
            }
        }
    }
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        // Calculate delta time in seconds
        float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
        if (deltaTime > 0.05f)
        {
            deltaTime = 0.05f;
        }

        mTicksCount = SDL_GetTicks();

        ProcessInput();
        UpdateGame(deltaTime);
        GenerateOutput();

        // Sleep to maintain frame rate
        int sleepTime = (1000 / FPS) - (SDL_GetTicks() - mTicksCount);
        if (sleepTime > 0)
        {
            SDL_Delay(sleepTime);
        }
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                Quit();
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(nullptr);

    for (auto actor : mActors)
    {
        actor->ProcessInput(state);
    }
}

void Game::UpdateGame(float deltaTime)
{
    // Update all actors and pending actors
    UpdateActors(deltaTime);

    // Update camera position
    UpdateCamera();
}

void Game::UpdateActors(float deltaTime)
{
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    for (auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == ActorState::Destroy)
        {
            deadActors.emplace_back(actor);
        }
    }

    for (auto actor : deadActors)
    {
        delete actor;
    }
}

void Game::UpdateCamera()
{
    if (!mMario)
    {
        return;
    }

    // 2. Pega a posição do Mario
    Vector2 marioPos = mMario->GetPosition();

    // 1. Trava o Y da câmera no topo do nível (Y=0)
    // (O sistema de coordenadas da sua projeção começa com Y=0 no topo)
    mCameraPos.y = 0.0f;

    // 2. Define onde a câmera "gostaria" de estar
    // Para centralizar o Mario, o canto esquerdo (mCameraPos.x)
    // deve ser a posição do Mario MENOS metade da tela.
    float halfScreenWidth = WINDOW_WIDTH / 2.0f;
    mCameraPos.x = marioPos.x - halfScreenWidth;

    // 3. Trava a câmera na borda ESQUERDA (x = 0)
    // A câmera não pode ir para a esquerda de 0.
    mCameraPos.x = std::max(0.0f, mCameraPos.x);

    // 4. Trava a câmera na borda DIREITA
    // A câmera não pode ir além do fim do nível.
    float levelWidth = LEVEL_WIDTH * TILE_SIZE;
    mCameraPos.x = std::min(mCameraPos.x, levelWidth - WINDOW_WIDTH);
}

void Game::AddActor(Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end())
    {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddDrawable(class DrawComponent *drawable)
{
    mDrawables.emplace_back(drawable);

    std::sort(mDrawables.begin(), mDrawables.end(),[](DrawComponent* a, DrawComponent* b) {
        return a->GetDrawOrder() < b->GetDrawOrder();
    });
}

void Game::RemoveDrawable(class DrawComponent *drawable)
{
    auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
    mDrawables.erase(iter);
}

void Game::AddCollider(class AABBColliderComponent* collider)
{
    mColliders.emplace_back(collider);
}

void Game::RemoveCollider(AABBColliderComponent* collider)
{
    auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
    mColliders.erase(iter);
}

void Game::GenerateOutput()
{
    // Clear back buffer
    mRenderer->Clear();

    Texture* bgTexture = mRenderer->GetTexture("../Assets/Sprites/Background.png");

    if (bgTexture)
    {
        // 3. Pega as dimensões REAIS da textura do fundo
        float bgWidth = static_cast<float>(bgTexture->GetWidth());
        float bgHeight = static_cast<float>(bgTexture->GetHeight());

        // 4. Calcula a posição do CENTRO do fundo NO MUNDO
        // Se o nível começa em (0,0), o centro está em (largura/2, altura/2)
        Vector2 bgPos(bgWidth / 2.0f, bgHeight / 2.0f);

        // 5. O TAMANHO do objeto a ser desenhado é o tamanho total da textura
        Vector2 bgSize(bgWidth, bgHeight);

        // 6. Desenha o fundo usando suas coordenadas de MUNDO
        mRenderer->DrawTexture(
            bgPos,                  // Posição do CENTRO do fundo no MUNDO
            bgSize,                 // Tamanho TOTAL do fundo
            0.0f,                   // Rotação
            Vector3(1.0f, 1.0f, 1.0f), // Cor (branco)
            bgTexture,              // A textura grande
            Vector4(0.0f, 0.0f, 1.0f, 1.0f), // textureRect (usa a textura inteira)
            GetCameraPos(),         // <<--- PASSA A CÂMERA ATUAL
            false,                  // flip
            1.0f                    // textureFactor
        );
    }
    else
    {
        SDL_Log("Erro: Nao foi possivel carregar a textura do fundo.");
    }

    for (auto drawable : mDrawables)
    {
        drawable->Draw(mRenderer);

        if(mIsDebugging)
        {
           // Call draw for actor components
              for (auto comp : drawable->GetOwner()->GetComponents())
              {
                comp->DebugDraw(mRenderer);
              }
        }
    }

    // Swap front buffer and back buffer
    mRenderer->Present();
}


void Game::Shutdown()
{
    while (!mActors.empty()) {
        delete mActors.back();
    }

    // Delete level data
    if (mLevelData) {
        for (int i = 0; i < LEVEL_HEIGHT; ++i) {
            delete[] mLevelData[i];
        }
        delete[] mLevelData;
        mLevelData = nullptr;
    }

    mRenderer->Shutdown();
    delete mRenderer;
    mRenderer = nullptr;

    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}