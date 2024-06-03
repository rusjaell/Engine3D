#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"

struct Material 
{
    Shared<Texture> diffuseMap;
    Shared<Texture> specularMap;
    Shared<Texture> normalMap;
    Shared<Texture> ambientHeightMap;

    glm::vec4 defaultDiffuseColor;
    glm::vec4 defaultAmbientColor;
    float shininess;
};

#endif // !MATERIAL_H
