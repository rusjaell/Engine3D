#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"
#include "Shader.h"

struct Material 
{
    void Use(const Shared<Shader>& shader)
    {
        if (shader->HasUniform("u_Material.ambientColor")) {
            shader->SetUniform3f("u_Material.ambientColor", ambientColor);
        }

        if (shader->HasUniform("u_Material.diffuseColor")) {
            shader->SetUniform3f("u_Material.diffuseColor", diffuseColor);
        }

        if (shader->HasUniform("u_Material.specularColor")) {
            shader->SetUniform3f("u_Material.specularColor", specularColor);
        }

        textureSlot = 0;
        if (diffuseTexture != nullptr && shader->HasUniform("u_Material.diffuseTexture"))
        {
            diffuseTexture->Bind(textureSlot);
            shader->SetUniform1i("u_Material.diffuseTexture", textureSlot);
            textureSlot++;
        }

        if (ambientTexture != nullptr && shader->HasUniform("u_Material.ambientTexture"))
        {
            ambientTexture->Bind(textureSlot);
            shader->SetUniform1i("u_Material.ambientTexture", textureSlot);
            textureSlot++;
        }

        if (specularTexture != nullptr && shader->HasUniform("u_Material.specularTexture"))
        {
            specularTexture->Bind(textureSlot);
            shader->SetUniform1i("u_Material.specularTexture", textureSlot);
            textureSlot++;
        }

        if (normalTexture != nullptr && shader->HasUniform("u_Material.normalTexture"))
        {
            normalTexture->Bind(textureSlot);
            shader->SetUniform1i("u_Material.normalTexture", textureSlot);
            textureSlot++;
        }

        if (alphaTexture != nullptr && shader->HasUniform("u_Material.alphaTexture"))
        {
            alphaTexture->Bind(textureSlot);
            shader->SetUniform1i("u_Material.alphaTexture", textureSlot);
            textureSlot++;
        }

        if (displacementTexture != nullptr && shader->HasUniform("u_Material.displacementTexture"))
        {
            displacementTexture->Bind(textureSlot);
            shader->SetUniform1i("u_Material.displacementTexture", textureSlot);
            textureSlot++;
        }
    }

    unsigned int materialId;
    std::string name;

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;

    Shared<Texture> diffuseTexture;
    Shared<Texture> ambientTexture;
    Shared<Texture> specularTexture;
    Shared<Texture> normalTexture;
    Shared<Texture> alphaTexture;
    Shared<Texture> displacementTexture;

    int textureSlot;
};

#endif // !MATERIAL_H
