#include "pch.h"
#include "Mesh.h"
#include "Application.h"

Mesh::Mesh(std::vector<VertexData> vertices, std::vector<unsigned int> indices, const Shared<Material>& material)
{
    vertices_ = vertices;
    //indices_ = indices;
    material_ = material;

    size_ = indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(VertexData), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    if (size_ <= 255) {
        std::vector<unsigned char> byteIndices(indices.begin(), indices.end());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteIndices.size() * sizeof(unsigned char), &byteIndices[0], GL_STATIC_DRAW);
        type_ = GL_UNSIGNED_BYTE;
    }
    else if (size_ <= 65535) {
        std::vector<unsigned short> shortIndices(indices.begin(), indices.end());
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, shortIndices.size() * sizeof(unsigned short), &shortIndices[0], GL_STATIC_DRAW);
        type_ = GL_UNSIGNED_SHORT;
    }
    else {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        type_ = GL_UNSIGNED_INT;
    }

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));

    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));

    // vertex normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));

    glBindVertexArray(0);
}

void Mesh::Draw(const Shared<Shader>& shader)
{
    material_->Use(shader);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, size_, type_, 0);

    Application::draws_++;
    Application::vertices += vertices_.size();
}