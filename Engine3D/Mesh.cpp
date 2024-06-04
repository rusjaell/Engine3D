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

    CalculateAABB();
}

void Mesh::CalculateAABB() {
    if (vertices_.empty()) {
        aabb_.minPoint = { 0.0f, 0.0f, 0.0f };
        aabb_.maxPoint = { 0.0f, 0.0f, 0.0f };
        return;
    }

    aabb_.minPoint = aabb_.maxPoint = vertices_[0].position;

    for (const auto& vertex : vertices_) 
    {
        aabb_.minPoint.x = std::min(aabb_.minPoint.x, vertex.position.x);
        aabb_.minPoint.y = std::min(aabb_.minPoint.y, vertex.position.y);
        aabb_.minPoint.z = std::min(aabb_.minPoint.z, vertex.position.z);

        aabb_.maxPoint.x = std::max(aabb_.maxPoint.x, vertex.position.x);
        aabb_.maxPoint.y = std::max(aabb_.maxPoint.y, vertex.position.y);
        aabb_.maxPoint.z = std::max(aabb_.maxPoint.z, vertex.position.z);
    }
}

void Mesh::Draw(const Shared<Shader>& shader)
{
  /*  unsigned int textureSlot = 0;
    
    if (material_->diffuseTexture != nullptr) {
        material_->diffuseTexture->Bind(textureSlot);
        shader->SetUniform1i("u_Diffuse", textureSlot);
        textureSlot++;
    }

    if (material_->roughnessMap != nullptr) {
        material_->roughnessMap->Bind(textureSlot);
        shader->SetUniform1i("u_Roughness", textureSlot);
        textureSlot++;
    }

    if (material_->normalMap != nullptr) {
        material_->normalMap->Bind(textureSlot);
        shader->SetUniform1i("u_Normal", textureSlot);
        textureSlot++;
    }

    if (material_->ambientOcclusionMap != nullptr) {
        material_->ambientOcclusionMap->Bind(textureSlot);
        shader->SetUniform1i("u_Ambient", textureSlot);
        textureSlot++;
    }

    if (material_->specularMap != nullptr) {
        material_->specularMap->Bind(textureSlot);
        shader->SetUniform1i("u_Specular", textureSlot);
        textureSlot++;
    }

    if (material_->metallicMap != nullptr) {
        material_->metallicMap->Bind(textureSlot);
        shader->SetUniform1i("u_Metallic", textureSlot);
        textureSlot++;
    }

    shader->SetUniform4f("u_AlbedoColor", material_->diffuseColor);
    shader->SetUniform4f("u_AmbientColor", material_->ambientColor);*/

    material_->Use(shader);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);

    //for (int i = 0; i < 6; ++i) {
    //    glActiveTexture(GL_TEXTURE0 + i);
    //    glBindTexture(GL_TEXTURE_2D, 0);
    //}

    Application::draws_++;
    Application::vertices += vertices_.size();
}