#include "pch.h"
#include "Mesh.h"
#include "Application.h"

Mesh::Mesh(std::vector<VertexData> vertices, std::vector<unsigned int> indices, const Shared<Material>& material)
{
    vertices_ = vertices;
    indices_ = indices;
    material_ = material;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(VertexData), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));

    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, uv));

    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, normal));

    glBindVertexArray(0);
}

void Mesh::Draw(const Shared<Shader>& shader)
{
    if (material_->diffuseMap != nullptr) {
        material_->diffuseMap->Bind(0);
        shader->SetUniform1i("u_Diffuse", 0);
    }

    if (material_->specularMap != nullptr) {
        material_->specularMap->Bind(1);
        shader->SetUniform1i("u_Specular", 1);
    }

    if (material_->normalMap != nullptr) {
        material_->normalMap->Bind(2);
        shader->SetUniform1i("u_Normal", 2);
    }

    shader->SetUniform4f("u_AlbedoColor", material_->defaultDiffuseColor);
    shader->SetUniform4f("u_AmbientColor", material_->defaultAmbientColor);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);

    Application::draws_++;
    Application::vertices += vertices_.size();
}