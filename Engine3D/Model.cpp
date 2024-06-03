#include "pch.h"
#include "Model.h"

void Model::Draw(const Shared<Shader>& shader)
{
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}