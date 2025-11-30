//
// Created by Lucas N. Ferreira on 28/09/23.
//

#include "AnimatorComponent.h"
#include <fstream>
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Json.h"
#include "../../Renderer/Texture.h"

AnimatorComponent::AnimatorComponent(class Actor* owner,
                                     const std::string& texPath,
                                     const std::string& dataPath,
                                     int width,
                                     int height,
                                     int drawOrder)
    : DrawComponent(owner, drawOrder),
      mAnimTimer(0.0f),
      mIsPaused(false),
      mWidth(width),
      mHeight(height),
      mTextureFactor(1.0f) {
  Renderer* renderer = mOwner->GetGame()->GetRenderer();
  mSpriteTexture = renderer->GetTexture(texPath);
  LoadSpriteSheetData(dataPath);
}

AnimatorComponent::~AnimatorComponent() {
  mAnimations.clear();
  mSpriteSheetData.clear();
}

void AnimatorComponent::SetTexture(Texture* texture) {
  mSpriteTexture = texture;
}

bool AnimatorComponent::LoadSheet(const std::string& texturePath,
                                  const std::string& dataPath) {
  // 1. Tenta carregar a nova textura
  Texture* newTexture =
      mOwner->GetGame()->GetRenderer()->GetTexture(texturePath);
  if (!newTexture) {
    SDL_Log("Falha ao carregar nova textura da folha: %s", texturePath.c_str());
    return false;
  }
  mSpriteTexture = newTexture;

  mSpriteSheetData.clear();
  mAnimations.clear();
  mAnimName = "";
  mAnimTimer = 0.0f;

  if (!LoadSpriteSheetData(dataPath)) {
    SDL_Log("Falha ao carregar dados da nova folha: %s", dataPath.c_str());
    return false;
  }

  SDL_Log("Nova folha de sprites carregada com sucesso: %s",
          texturePath.c_str());
  return true;
}

bool AnimatorComponent::LoadSpriteSheetData(const std::string& dataPath) {
  // Load sprite sheet data and return false if it fails
  std::ifstream spriteSheetFile(dataPath);

  if (!spriteSheetFile.is_open()) {
    // SDL_Log("Failed to open sprite sheet data file: %s (called from %s)",
    //         dataPath.c_str(), mOwner->GetName().c_str());
    return false;
  }

  try {
    nlohmann::json spriteSheetData = nlohmann::json::parse(spriteSheetFile);

    if (spriteSheetData.is_null()) {
      SDL_Log("Failed to parse sprite sheet data file: %s", dataPath.c_str());
      return false;
    }

    auto textureWidth =
        static_cast<float>(spriteSheetData["meta"]["size"]["w"].get<int>());
    auto textureHeight =
        static_cast<float>(spriteSheetData["meta"]["size"]["h"].get<int>());

    for (const auto& frame : spriteSheetData["frames"]) {
      int x = frame["frame"]["x"].get<int>();
      int y = frame["frame"]["y"].get<int>();
      int w = frame["frame"]["w"].get<int>();
      int h = frame["frame"]["h"].get<int>();

      float u = static_cast<float>(x) / textureWidth;
      float v = static_cast<float>(y) / textureHeight;
      float uw = static_cast<float>(w) / textureWidth;
      float vh = static_cast<float>(h) / textureHeight;

      mSpriteSheetData.emplace_back(u, v, uw, vh);
    }
  } catch (const std::exception& e) {
    SDL_Log("JSON Parsing Error in %s: %s", dataPath.c_str(), e.what());
    return false;
  } catch (...) {
    SDL_Log("Unknown error parsing JSON file: %s", dataPath.c_str());
    return false;
  }

  return true;
}

void AnimatorComponent::Draw(Renderer* renderer) {
  if (!mIsVisible)
    return;

  const Vector2& worldPos = mOwner->GetPosition();
  const Vector2& cameraPos = mOwner->GetGame()->GetCameraPos();
  Vector4 texRect(0.0f, 0.0f, 1.0f, 1.0f);  // Default: textura inteira

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

  renderer->DrawTexture(worldPos, mOwner->GetScale(), mOwner->GetRotation(),
                        Vector4(1.0f, 1.0f, 1.0f, 1.0f), mSpriteTexture,
                        texRect, cameraPos, mFlipHorizontal, 1.0f,
                        mFlipVertical);
}
void AnimatorComponent::Update(float deltaTime) {
  if (mIsPaused) {
    return;
  }

  if (mAnimations.empty()) {
    return;
  }

  auto animIter = mAnimations.find(mAnimName);
  if (animIter == mAnimations.end()) {
    return;
  }

  const std::vector<int>& animFrames = animIter->second;

  float numFrames = static_cast<float>(animFrames.size());

  if (numFrames == 0.0f) {
    return;
  }

  mAnimTimer += mAnimFPS * deltaTime;

  if (mAnimTimer >= numFrames) {
    mAnimTimer = std::fmod(mAnimTimer, numFrames);
  }
}

void AnimatorComponent::SetAnimation(const std::string& name) {
  if (mAnimName == name) {
    return;
  }

  mAnimName = name;
  Update(0.0f);
}

void AnimatorComponent::AddAnimation(const std::string& name,
                                     const std::vector<int>& spriteNums) {
  mAnimations.emplace(name, spriteNums);
}