#include "Texture.h"
#include <iostream>

Texture::Texture() : mTextureID(0), mWidth(0), mHeight(0) {}

Texture::~Texture() {}

bool Texture::Load(const std::string& fileName) {
  SDL_Surface* surface = IMG_Load(fileName.c_str());
  if (surface == nullptr) {
    std::cerr << "Falha ao carregar a imagem: " << fileName
              << ". Erro SDL: " << IMG_GetError() << std::endl;
    return false;
  }

  SDL_Surface* formattedSurface =
      SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

  if (formattedSurface == nullptr) {
    std::cerr << "Falha ao converter a superfície para o formato RGBA: "
              << SDL_GetError() << std::endl;
    SDL_FreeSurface(surface);  // Limpa a superfície original antes de sair
    return false;
  }
  // ==========================================================

  mWidth = formattedSurface->w;
  mHeight = formattedSurface->h;

  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, formattedSurface->pitch / 4);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA,
               GL_UNSIGNED_BYTE,
               formattedSurface->pixels  // Agora usamos a superfície convertida
  );
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  SDL_FreeSurface(surface);
  SDL_FreeSurface(formattedSurface);

  // Configura os filtros para pixel art (GL_NEAREST)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  std::cout << "Textura " << fileName
            << " carregada com sucesso (ID: " << mTextureID << ")."
            << std::endl;
  return true;
}
void Texture::Unload() {
  if (mTextureID != 0) {
    glDeleteTextures(1, &mTextureID);
    mTextureID = 0;
    mWidth = 0;
    mHeight = 0;
  }
}

void Texture::SetActive(int index) const {
  glActiveTexture(GL_TEXTURE0 + index);

  glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::CreateFromSurface(SDL_Surface* surface) {
  mWidth = surface->w;
  mHeight = surface->h;

  // Generate a GL texture
  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, surface->pitch / 4);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, surface->pixels);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

  // Use linear filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
