//
// Created by Lucas N. Ferreira on 22/05/25.
//

#include "UIButton.h"
#include "../Renderer/Texture.h"

UIButton::UIButton(class Game* game,
                   std::function<void()> onClick,
                   const std::string& text,
                   class Font* font,
                   const Vector2& offset,
                   float scale,
                   float angle,
                   int pointSize,
                   const unsigned wrapLength,
                   int drawOrder)
    : UIText(game,
             text,
             font,
             offset,
             scale,
             angle,
             pointSize,
             wrapLength,
             drawOrder),
      mOnClick(onClick),
      mHighlighted(false) {}

UIButton::~UIButton() {}

void UIButton::OnClick() {
  // Call attached handler, if it exists
  if (mOnClick) {
    mOnClick();
  }
}

void UIButton::Draw(class Shader* shader) {
  // Simulate alpha with brightness for Vector3
  Vector3 highlightColor = mBackgroundColor * (mHighlighted ? 1.2f : 0.7f);

  // Temporarily change background color for highlight effect
  Vector3 oldBgColor = mBackgroundColor;
  mBackgroundColor = highlightColor;

  UIText::Draw(shader);

  // Restore original color
  mBackgroundColor = oldBgColor;
}
