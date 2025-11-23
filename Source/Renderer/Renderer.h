#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "../Math.h"
#include "Texture.h"
#include "VertexArray.h"

enum class RendererMode { TRIANGLES, LINES };

class Renderer {
 public:
  Renderer(SDL_Window* window);
  ~Renderer();

  bool Initialize(float width, float height);
  void Shutdown();

  void DrawRect(const Vector2& position,
                const Vector2& size,
                float rotation,
                const Vector3& color,
                const Vector2& cameraPos,
                RendererMode mode);

  void DrawTexture(const Vector2& position,
                   const Vector2& size,
                   float rotation,
                   const Vector3& color,
                   Texture* texture,
                   const Vector4& textureRect = Vector4::UnitRect,
                   const Vector2& cameraPos = Vector2::Zero,
                   bool flip = false,
                   float textureFactor = 1.0f);

  void DrawGeometry(const Vector2& position,
                    const Vector2& size,
                    float rotation,
                    const Vector3& color,
                    const Vector2& cameraPos,
                    VertexArray* vertexArray,
                    RendererMode mode);

  void AddUIElement(class UIElement* comp);
  void RemoveUIElement(class UIElement* comp);

  void Clear();
  void Present();

  // Getters
  void DrawDebugRect();
  class Texture* GetTexture(const std::string& fileName);
  class Shader* GetBaseShader() const { return mBaseShader; }

 private:
  void Draw(RendererMode mode,
            const Matrix4& modelMatrix,
            const Vector2& cameraPos,
            VertexArray* vertices,
            const Vector3& color,
            Texture* texture = nullptr,
            const Vector4& textureRect = Vector4::UnitRect,
            float textureFactor = 1.0f);

  bool LoadShaders();
  void CreateSpriteVerts();

  // Game
  class Game* mGame;

  // Basic shader
  class Shader* mBaseShader;

  // Sprite vertex array
  class VertexArray* mSpriteVerts;

  // Window
  SDL_Window* mWindow;

  // OpenGL context
  SDL_GLContext mContext;

  // Ortho projection for 2D shaders
  Matrix4 mOrthoProjection;

  // UI screens to draw
  std::vector<class UIElement*> mUIComps;

  // Map of textures loaded
  std::unordered_map<std::string, class Texture*> mTextures;
};