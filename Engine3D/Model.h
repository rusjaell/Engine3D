#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include "Mesh.h"

struct Model
{
    void Draw(const Shared<Shader>& shader);

    glm::vec3 position_;
    glm::quat rotation_;
    glm::vec3 scale_;

    std::vector<Shared<Mesh>> meshes;
};

#endif