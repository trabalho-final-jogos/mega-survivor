#include "VertexArray.h"
#include <GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
                         const unsigned int* indices, unsigned int numIndices)
    : mNumVerts(numVerts)
    , mNumIndices(numIndices)
    , mVertexArray(0)
    , mVertexBuffer(0)
    , mIndexBuffer(0)
{
    // 1. Cria o Vertex Array Object (VAO)
    // O VAO armazena toda a configuração de estado dos buffers abaixo.
    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    // 2. Cria o Vertex Buffer Object (VBO) para os dados dos vértices
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

    // 3. Envia os dados dos vértices (da CPU para a GPU)
    // O tamanho total é o número de vértices * 4 floats por vértice * tamanho de um float.
    glBufferData(GL_ARRAY_BUFFER,
                 numVerts * 4 * sizeof(float),
                 verts,
                 GL_STATIC_DRAW);

    // 4. Cria o Index Buffer Object (IBO/EBO) para os índices
    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

    // 5. Envia os dados dos índices (da CPU para a GPU)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 numIndices * sizeof(unsigned int),
                 indices,
                 GL_STATIC_DRAW);

    // --- CONFIGURAÇÃO DOS ATRIBUTOS (A PARTE MAIS IMPORTANTE) ---

    // O "stride" (passo) é o tamanho total de um único vértice em bytes.
    // No nosso caso: 2 floats (posição) + 2 floats (textura) = 4 floats.
    GLsizei stride = 4 * sizeof(float);

    // 6. Atributo de Posição (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,          // location = 0 (conforme o shader)
        2,          // Número de componentes (vec2: x, y)
        GL_FLOAT,   // Tipo dos dados
        GL_FALSE,   // Não normalizar
        stride,     // Tamanho total de um vértice (para saber onde começa o próximo)
        0           // Offset (em bytes) de onde este atributo começa (começa no byte 0)
    );

    // 7. Atributo de Coordenada de Textura (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,          // location = 1 (conforme o shader)
        2,          // Número de componentes (vec2: u, v)
        GL_FLOAT,   // Tipo dos dados
        GL_FALSE,   // Não normalizar
        stride,     // Tamanho total de um vértice
        (void*)(2 * sizeof(float)) // Offset (começa APÓS os 2 floats da posição)
    );
}
VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive() const
{
	glBindVertexArray(mVertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
}
