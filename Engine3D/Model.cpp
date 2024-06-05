#include "pch.h"
#include "Model.h"

void Model::Draw(const Shared<Shader>& shader)
{
    glm::mat4 model(1.0);
    model = glm::translate(model, position_);
    model *= glm::toMat4(rotation_);
    model = glm::scale(model, scale_);

    shader->SetUniformMatrix4f("u_Model", model);
    
    for (const Shared<Mesh>& mesh : meshes) {
        mesh->Draw(shader);
    }
}