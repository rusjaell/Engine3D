#ifndef MATERIAL_H
#define MATERIAL_H

#include "Texture.h"

struct Material 
{
    Shared<Texture> albedoMap;
    Shared<Texture> roughnessMap;
    Shared<Texture> normalMap;
    Shared<Texture> ambientOcclusionMap; 
    Shared<Texture> specularMap;
    Shared<Texture> metallicMap;

    glm::vec4 defaultAlbedoColor;
    glm::vec4 defaultAmbientColor;
    float shininess;
};

#endif // !MATERIAL_H
