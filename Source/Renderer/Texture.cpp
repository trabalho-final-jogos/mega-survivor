#include "Texture.h"
#include <iostream>

Texture::Texture()
: mTextureID(0)
, mWidth(0)
, mHeight(0)
{
}

Texture::~Texture()
{
}

bool Texture::Load(const std::string& fileName)
{
    // Carrega a imagem original do arquivo
    SDL_Surface* surface = IMG_Load(fileName.c_str());
    if (surface == nullptr)
    {
        std::cerr << "Falha ao carregar a imagem: " << fileName << ". Erro SDL: " << IMG_GetError() << std::endl;
        return false;
    }

    // ================== A CORREÇÃO ESTÁ AQUI ==================
    // Em vez de criar uma struct SDL_PixelFormat manualmente,
    // usamos SDL_ConvertSurfaceFormat para converter a superfície para um
    // formato padrão (RGBA de 32 bits) que o OpenGL entende bem.
    // É mais simples, mais seguro e mais portável.
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);

    if (formattedSurface == nullptr)
    {
        std::cerr << "Falha ao converter a superfície para o formato RGBA: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface); // Limpa a superfície original antes de sair
        return false;
    }
    // ==========================================================

    // Armazena a largura e altura da textura (da superfície já formatada)
    mWidth = formattedSurface->w;
    mHeight = formattedSurface->h;

    // Gera o ID da textura e faz o bind
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    // Envia os pixels da superfície formatada para a GPU
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        mWidth,
        mHeight,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        formattedSurface->pixels // Agora usamos a superfície convertida
    );

    // Libera as duas superfícies da memória da CPU, pois os dados já estão na GPU
    SDL_FreeSurface(surface);
    SDL_FreeSurface(formattedSurface);

    // Configura os filtros para pixel art (GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    std::cout << "Textura " << fileName << " carregada com sucesso (ID: " << mTextureID << ")." << std::endl;
    return true;
}
void Texture::Unload()
{
    if (mTextureID != 0)
    {
        glDeleteTextures(1, &mTextureID);
        mTextureID = 0;
        mWidth = 0;
        mHeight = 0;
    }
}

void Texture::SetActive(int index) const
{
    glActiveTexture(GL_TEXTURE0 + index);

    glBindTexture(GL_TEXTURE_2D, mTextureID);
}
