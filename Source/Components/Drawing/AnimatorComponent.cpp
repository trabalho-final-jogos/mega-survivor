//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AnimatorComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Json.h"
#include "../../Renderer/Texture.h"
#include <fstream>

AnimatorComponent::AnimatorComponent(class Actor* owner, const std::string &texPath, const std::string &dataPath,
                                     int width, int height, int drawOrder)
        :DrawComponent(owner,  drawOrder)
        ,mAnimTimer(0.0f)
        ,mIsPaused(false)
        ,mWidth(width)
        ,mHeight(height)
        ,mTextureFactor(1.0f)
{
    Renderer* renderer = mOwner->GetGame()->GetRenderer();
    mSpriteTexture = renderer->GetTexture(texPath);
    LoadSpriteSheetData(dataPath);
}

AnimatorComponent::~AnimatorComponent()
{
    mAnimations.clear();
    mSpriteSheetData.clear();
}

void AnimatorComponent::SetTexture(Texture *texture) {
    mSpriteTexture = texture;
}

bool AnimatorComponent::LoadSheet(const std::string& texturePath, const std::string& dataPath)
{
    // 1. Tenta carregar a nova textura
    Texture* newTexture = mOwner->GetGame()->GetRenderer()->GetTexture(texturePath);
    if (!newTexture)
    {
        SDL_Log("Falha ao carregar nova textura da folha: %s", texturePath.c_str());
        return false; // Falha se a textura não carregar
    }
    mSpriteTexture = newTexture; // Atualiza o ponteiro da textura

    // 2. Limpa os dados antigos da folha e as animações
    mSpriteSheetData.clear();
    mAnimations.clear();
    mAnimName = ""; // Reseta o nome da animação atual
    mAnimTimer = 0.0f; // Reseta o timer

    // 3. Carrega os novos dados da folha a partir do JSON
    // (A função LoadSpriteSheetData preencherá mSpriteSheetData)
    if (!LoadSpriteSheetData(dataPath))
    {
        SDL_Log("Falha ao carregar dados da nova folha: %s", dataPath.c_str());
        // Poderia reverter para a textura antiga aqui se quisesse ser mais robusto
        return false; // Falha se o JSON não carregar/parsear
    }

    SDL_Log("Nova folha de sprites carregada com sucesso: %s", texturePath.c_str());
    return true;
}

bool AnimatorComponent::LoadSpriteSheetData(const std::string& dataPath)
{
    // Load sprite sheet data and return false if it fails
    std::ifstream spriteSheetFile(dataPath);

    if (!spriteSheetFile.is_open()) {
        SDL_Log("Failed to open sprite sheet data file: %s", dataPath.c_str());
        return false;
    }

    nlohmann::json spriteSheetData = nlohmann::json::parse(spriteSheetFile);

    if (spriteSheetData.is_null()) {
        SDL_Log("Failed to parse sprite sheet data file: %s", dataPath.c_str());
        return false;
    }

    auto textureWidth = static_cast<float>(spriteSheetData["meta"]["size"]["w"].get<int>());
    auto textureHeight = static_cast<float>(spriteSheetData["meta"]["size"]["h"].get<int>());

    for(const auto& frame : spriteSheetData["frames"]) {

        int x = frame["frame"]["x"].get<int>();
        int y = frame["frame"]["y"].get<int>();
        int w = frame["frame"]["w"].get<int>();
        int h = frame["frame"]["h"].get<int>();

        mSpriteSheetData.emplace_back(static_cast<float>(x)/textureWidth, static_cast<float>(y)/textureHeight,
                                      static_cast<float>(w)/textureWidth, static_cast<float>(h)/textureHeight);
    }

    return true;
}


void AnimatorComponent::Draw(Renderer* renderer) {
    if (!mIsVisible) return;

    const Vector2& worldPos = mOwner->GetPosition();
    const Vector2& cameraPos = mOwner->GetGame()->GetCameraPos();
    Vector4 texRect(0.0f, 0.0f, 1.0f, 1.0f); // Default: textura inteira

    auto animIter = mAnimations.find(mAnimName);
    if (animIter != mAnimations.end()) {
        const std::vector<int>& frames = animIter->second;

        if (!frames.empty()) {
            int frameIndex = frames[static_cast<int>(mAnimTimer) % frames.size()];

            if (frameIndex >= 0 && frameIndex < mSpriteSheetData.size()) {
                texRect = mSpriteSheetData[frameIndex];

            } else {
            }
        } else {
        }
    } else {
    }

    renderer->DrawTexture(
        worldPos,
        mOwner->GetScale(),
        0.0f,
        Vector3(1.0f, 1.0f, 1.0f),
        mSpriteTexture,
        texRect,
        cameraPos,
        mFlipHorizontal,
        1.0f
    );
}
void AnimatorComponent::Update(float deltaTime)
{
    if (mIsPaused)
    {
        return;
    }

    // 2. Verifica se temos alguma animação carregada.
    if (mAnimations.empty())
    {
        return;
    }

    // 3. Tenta encontrar a animação atual no mapa
    auto animIter = mAnimations.find(mAnimName);
    if (animIter == mAnimations.end())
    {
        // Se a animação nomeada não existe, não faz nada
        return;
    }

    // 4. Pega o vetor de quadros da animação atual
    const std::vector<int>& animFrames = animIter->second;

    // Converte o número de quadros para float
    float numFrames = static_cast<float>(animFrames.size());

    // Se a animação atual não tiver quadros, não há o que atualizar
    if (numFrames == 0.0f)
    {
        return;
    }

    // 5. Avança o contador de animação (conforme Aula 11 [cite: 257])
    // Isso move o timer para o próximo "quadro" (ex: de 1.1 para 1.3)
    mAnimTimer += mAnimFPS * deltaTime;

    // 6. Verifica se o timer ultrapassou o número total de quadros (conforme Aula 11 [cite: 258])
    if (mAnimTimer >= numFrames)
    {
        // Reinicia o timer usando o módulo (fmod), garantindo um loop contínuo.
        // Isso mantém o "excesso" de tempo para uma animação suave.
        // (Ex: se numFrames=3 e mAnimTimer=3.1, o novo timer será 0.1)
        mAnimTimer = std::fmod(mAnimTimer, numFrames);
    }
}


void AnimatorComponent::SetAnimation(const std::string& name)
{
    if (mAnimName == name)
    {
        return;
    }

    // 1. Atualiza o nome da animação para o novo valor
    mAnimName = name;

    // 2. Chama Update(0.0f)
    // Conforme solicitado, isso força o timer da animação (mAnimTimer)
    // a ser reavaliado imediatamente com o número de quadros
    // da *nova* animação, aplicando a lógica de loop (fmod)
    // para evitar um índice de quadro inválido.
    Update(0.0f);
}

void AnimatorComponent::AddAnimation(const std::string& name, const std::vector<int>& spriteNums)
{
    mAnimations.emplace(name, spriteNums);
}