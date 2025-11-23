//
// Created by Lucas N. Ferreira on 22/05/25.
//

#include "UIText.h"
#include "../Renderer/Font.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Texture.h"

UIText::UIText(class Game* game,
               const std::string& text,
               class Font* font,
               const Vector2& offset,
               float scale,
               float angle,
               int pointSize,
               const unsigned wrapLength,
               int drawOrder)
    : UIImage(game, offset, scale, angle, drawOrder),
      mFont(font),
      mPointSize(pointSize),
      mWrapLength(wrapLength),
      mTextColor(Color::White),
      mBackgroundColor(0.0f, 0.0f, 1.0f),
      mMargin(Vector2(50.0f, 10.f)) {
  SetText(text);
}

UIText::~UIText() {}

void UIText::SetText(const std::string& text) {
  // Clear out previous title texture if it exists
  if (mTexture) {
    mTexture->Unload();
    delete mTexture;
    mTexture = nullptr;
  }

  // Create texture for title
  mText = text;
  mTexture = mFont->RenderText(mText, mTextColor, mPointSize, mWrapLength);
}

void UIText::SetTextColor(const Vector3& color) {
  // Clear out previous title texture if it exists
  if (mTexture) {
    mTexture->Unload();
    delete mTexture;
    mTexture = nullptr;
  }

  mTextColor = color;
  mTexture = mFont->RenderText(mText, mTextColor, mPointSize, mWrapLength);
}

void UIText::Draw(class Shader* shader) {
  if (!mTexture || !mIsVisible)
    return;

  // 1. BACKGROUND (solid color quad - NO TEXTURE)
  Matrix4 bgScale = Matrix4::CreateScale(
      (static_cast<float>(mTexture->GetWidth()) + mMargin.x * 2.0f) * mScale,
      (static_cast<float>(mTexture->GetHeight()) + mMargin.y * 2.0f) * mScale,
      1.0f);
  Matrix4 bgTrans =
      Matrix4::CreateTranslation(Vector3(mOffset.x, mOffset.y, 0.0f));
  Matrix4 bgWorld = bgScale * bgTrans;

  shader->SetMatrixUniform("uWorldTransform", bgWorld);
  shader->SetFloatUniform("uTextureFactor", 0.0f);  // Solid color [file:9]
  shader->SetVectorUniform("uColor",
                           Vector3(mBackgroundColor));  // Blue BG [file:9]
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                 nullptr);  // NO texture active!

  // 2. TEXT (texture over background)
  Matrix4 textScale = Matrix4::CreateScale(
      static_cast<float>(mTexture->GetWidth()) * mScale,
      static_cast<float>(mTexture->GetHeight()) * mScale, 1.0f);
  Matrix4 textTrans =
      Matrix4::CreateTranslation(Vector3(mOffset.x, mOffset.y, 0.0f));
  Matrix4 textWorld = textScale * textTrans;

  shader->SetMatrixUniform("uWorldTransform", textWorld);
  shader->SetFloatUniform("uTextureFactor", 1.0f);  // Full texture [file:9]
  shader->SetVectorUniform("uColor", mTextColor);   // White multiply [file:9]
  mTexture->SetActive();                            // Bind font texture
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);  // Draw text quad
}
