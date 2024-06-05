#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include "Material.h"

struct VertexData
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};

struct AABB {
    glm::vec3 minPoint;
    glm::vec3 maxPoint;
};

struct Mesh
{
    Mesh(std::vector<VertexData> vertices, std::vector<unsigned int> indices, const Shared<Material>& material);

    void Draw(const Shared< Shader>& shader);

//private:

    std::vector<VertexData> vertices_;
    std::vector<unsigned int> indices_;
    Shared<Material> material_;

    unsigned int VAO, VBO, EBO;
};

#endif