#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"
#include "Shader.h"
#include "Asset.h"

struct Material : public Asset
{
    void Use(const Shared<Shader>& shader)
    {
        glBindTextureUnit(0, 0);
        glBindTextureUnit(1, 0);
        glBindTextureUnit(2, 0);
        glBindTextureUnit(3, 0);
        glBindTextureUnit(4, 0);
        glBindTextureUnit(5, 0);

        shader->SetUniform1i("u_Material.diffuseTexture", 0);
        shader->SetUniform1i("u_Material.ambientTexture", 1);
        shader->SetUniform1i("u_Material.specularTexture", 2);
        shader->SetUniform1i("u_Material.normalTexture", 3);
        shader->SetUniform1i("u_Material.alphaTexture", 4);
        shader->SetUniform1i("u_Material.displacementTexture", 5);

        shader->SetUniform3f("u_Material.ambientColor", ambientColor);
        shader->SetUniform3f("u_Material.diffuseColor", diffuseColor);
        shader->SetUniform3f("u_Material.specularColor", specularColor);
        shader->SetUniform1f("u_Material.shininess", shininess);

        if (diffuseTexture != nullptr && shader->HasUniform("u_Material.diffuseTexture"))
        {
            diffuseTexture->Bind(0);
        }

        if (ambientTexture != nullptr && shader->HasUniform("u_Material.ambientTexture"))
        {
            ambientTexture->Bind(1);
        }

        if (specularTexture != nullptr && shader->HasUniform("u_Material.specularTexture"))
        {
            specularTexture->Bind(2);
        }

        if (normalTexture != nullptr && shader->HasUniform("u_Material.normalTexture"))
        {
            normalTexture->Bind(3);
        }

        if (alphaTexture != nullptr && shader->HasUniform("u_Material.alphaTexture"))
        {
            alphaTexture->Bind(4);
        }

        if (displacementTexture != nullptr && shader->HasUniform("u_Material.displacementTexture"))
        {
            displacementTexture->Bind(5);
        }
    }

    unsigned int materialId;
    std::string name;

    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;

    glm::vec3 specularColor;
    float shininess;

    Shared<Texture> diffuseTexture;
    Shared<Texture> ambientTexture;
    Shared<Texture> specularTexture;
    Shared<Texture> normalTexture;
    Shared<Texture> alphaTexture;
    Shared<Texture> displacementTexture;
};

#endif // !MATERIAL_H
