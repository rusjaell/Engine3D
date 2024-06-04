#ifndef ASSETLIBRARY_H
#define ASSETLIBRARY_H

#include "Texture.h"
#include "Shader.h"
#include "Material.h"

static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

class AssetLibrary
{
public:
    static Shared<Texture> LoadTexture(const std::string& path);
    static Shared<Shader> LoadShader(const std::string& path);
    static Shared<Material> LoadMaterial(const std::string& path);

    template<typename T>
    static Shared<T> GetAsset(AssetHandle handle);

private:
    static AssetHandle nextAssetHandle_;
    static std::map<AssetHandle, Shared<Asset>> assets_;
};

#endif
