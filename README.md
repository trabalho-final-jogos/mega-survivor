# Mega Survivors

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Projeto da disciplina de **Desenvolvimento de Jogos Digitais (DCC192)** - UFMG

`Mega Survivors` é um jogo de ação *survivors-like* (roguelite de sobrevivência contra hordas) inspirado no grande sucesso *Vampire Survivors*. A diferença? Trazemos toda a nostalgia e o universo clássico de **Mega Man** para a batalha.

## 🎮 Sobre o Jogo

Em `Mega Survivors`, você assume o controle de um personagem icônico da franquia Mega Man (como o próprio Blue Bomber ou seus aliados) e deve sobreviver a ondas intermináveis de robôs e inimigos clássicos do Dr. Wily.

Colete *parafusos* (experiência) dos inimigos derrotados para subir de nível e escolher novos *upgrades* e armas. Combine diferentes *Robot Master weapons* para criar sinergias devastadoras e se tornar a máquina de combate definitiva. O objetivo é simples: **sobreviver o máximo de tempo possível.**

## ✨ Features Principais

* **Sobrevivência contra Hordas:** Enfrente exércitos de inimigos que não param de crescer.
* **Temática Clássica:** Todos os sprites, músicas e efeitos sonoros são inspirados no universo 8-bit de Mega Man.
* **Progressão Roguelite:** Cada partida é diferente. Escolha entre uma seleção aleatória de armas e *upgrades* ao subir de nível.
* **Arsenal Icônico:** Desbloqueie e combine armas clássicas, como o *Metal Blade*, *Atomic Fire* e *Crash Bomber*.
* **[Se vocês planejam ter chefes, adicione]:** Chefes Desafiadores: Enfrente *Robot Masters* clássicos que aparecem para testar suas habilidades.

## 🛠️ Tecnologias Utilizadas

Este projeto foi desenvolvido **do zero (sem *game engine*)** utilizando:

* **Linguagem Principal:** C++ (Padrão C++17)
* **Gráficos e Janelas:** SDL2 (para gerenciamento de janelas, input e eventos)
* **Renderização:** OpenGL (para renderização 2D/3D acelerada)
* **Extensões OpenGL:** GLEW (para gerenciamento moderno de extensões OpenGL)
* **Áudio:** Bxfr (para geração de efeitos sonoros 8-bit)

## 🚀 Como Compilar e Jogar

**Requisito Mínimo:** Para compilar, você precisará ter o C++, CMake 3.10+ e todas as bibliotecas listadas acima instaladas.

[Explique aqui os passos de build. Exemplo abaixo usando CMake]

```bash
# 1. Clone o repositório
git clone git@github.com:trabalho-final-jogos/mega-survivor.git
cd mega-survivors

# 2. Crie um diretório de build
mkdir build
cd build

# 3. Configure o projeto (assumindo que as libs estão no path)
cmake ..

# 4. Compile o jogo
cmake --build .

# 5. Execute
./MegaSurvivors