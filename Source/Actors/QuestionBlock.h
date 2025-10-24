//
// Created by filipe on 10/22/25.
//
// QuestionBlock.h
#pragma once
#include "Block.h" // Inclui a classe base

// Forward declarations
class Texture;
class AnimatorComponent;

class QuestionBlock : public Block // Herda publicamente de Block
{
public:
    // Construtor: Recebe os mesmos parâmetros que Block e os repassa
    QuestionBlock(class Game* game, const std::string& texturePath, std::string name);

    // Sobrescreve (override) o método OnUpdate da classe Block
    void OnUpdate(float deltaTime) override;

    void SetAsEmpty();

    // Sobrescreve (override) o método StartBounce da classe Block
    void StartBounce() override;

private:
    bool mIsEmpty;             // Já foi atingido?
    Texture* mEmptyBlockTexture; // Ponteiro para a textura "vazia"
};