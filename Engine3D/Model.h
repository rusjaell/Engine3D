#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include "Mesh.h"

struct Model
{
    void Draw(const Shared<Shader>& shader);

    std::vector<Mesh> meshes;
};

#endif