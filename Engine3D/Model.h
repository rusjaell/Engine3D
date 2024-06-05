#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

struct Model
{
    void Draw(Camera& camera, const Shared<Shader>& shader);

    glm::vec3 position_;
    glm::quat rotation_;
    glm::vec3 scale_;

    std::vector<Shared<Mesh>> meshes;
};

#endif