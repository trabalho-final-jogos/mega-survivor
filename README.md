# Mega Survivors

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Projeto da disciplina de **Desenvolvimento de Jogos Digitais (DCC192)** - UFMG

`Mega Survivors` é um jogo de ação *survivors-like* (roguelite de sobrevivência contra hordas) inspirado no grande sucesso *Vampire Survivors*. A diferença? Trazemos toda a nostalgia e o universo clássico de **Mega Man** para a batalha.

## 🎮 Sobre o Jogo

Em `Mega Survivors`, você assume o controle de um personagem icônico da franquia Mega Man e deve sobreviver a ondas intermináveis de robôs e inimigos clássicos do Dr. Wily.

Colete *parafusos* (experiência) dos inimigos derrotados para subir de nível e escolher novos *upgrades* e armas. Combine diferentes *Robot Master weapons* para criar sinergias devastadoras e se tornar a máquina de combate definitiva. O objetivo é simples: **sobreviver o máximo de tempo possível.**

## ✨ Features Principais

*   **Sobrevivência contra Hordas:** Enfrente exércitos de inimigos como *Bats*, *Metalls* e outros robôs que não param de crescer.
*   **Temática Clássica:** Todos os sprites, músicas e efeitos sonoros são inspirados no universo 8-bit de Mega Man.
*   **Progressão Roguelite:** Cada partida é diferente. Escolha entre uma seleção aleatória de armas e *upgrades* ao subir de nível.
*   **Arsenal Icônico:** Desbloqueie e combine armas clássicas, como o *Metal Blade*, *Ice Slasher*, *Laser Beam*, *Boomerang* e *Aura*.
*   **Menu de Upgrades:** Utilize os recursos coletados para comprar melhorias permanentes na loja de upgrades.
*   **Seleção de Personagens:** Escolha seu herói favorito para a batalha.

## 🛠️ Tecnologias Utilizadas

Este projeto foi desenvolvido **do zero (sem *game engine*)** utilizando:

*   **Linguagem Principal:** C++ (Padrão C++17)
*   **Gráficos e Janelas:** SDL2 (para gerenciamento de janelas, input e eventos)
*   **Renderização:** OpenGL (para renderização 2D acelerada)
*   **Extensões OpenGL:** GLEW (para gerenciamento moderno de extensões OpenGL)
*   **Áudio:** SDL2_mixer e Bxfr (para geração de efeitos sonoros 8-bit)
*   **Fontes:** SDL2_ttf

## 🚀 Como Compilar e Jogar

### Pré-requisitos

Para compilar, você precisará ter o compilador C++, CMake 3.26+ e as bibliotecas de desenvolvimento instaladas.

#### Linux (Debian/Ubuntu)

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libglew-dev
```

### Compilando o Jogo

Siga os passos abaixo para compilar e executar o jogo:

```bash
# 1. Clone o repositório
git clone https://github.com/trabalho-final-jogos/mega-survivor.git
cd mega-survivor

# 2. Crie um diretório de build
mkdir -p build
cd build

# 3. Configure o projeto
cmake ..

# 4. Compile o jogo
cmake --build .

# 5. Execute
./mega-survivor
```

## 🕹️ Controles

*   **WASD:** Movimentação do personagem.
*   **Mouse:** Mira (para armas direcionais).
*   **ESC:** Pausa o jogo e abre o menu de pausa.

## 📄 Licença

Este projeto está licenciado sob a licença MIT.
