#include "pch.h"
#include "AssetLibrary.h"

#include "Texture.h"
#include "Material.h"

std::map<AssetHandle, Shared<Asset>> AssetLibrary::assets_;

Shared<Texture> AssetLibrary::LoadTexture(const std::string& path)
{
    return Shared<Texture>();
}

Shared<Shader> AssetLibrary::LoadShader(const std::string& path)
{
    return Shared<Shader>();
}

Shared<Material> AssetLibrary::LoadMaterial(const std::string& path)
{
    return Shared<Material>();
}

template<typename T>
Shared<T> AssetLibrary::GetAsset(AssetHandle handle) {
    auto it = assets_.find(handle);
    if (it != assets_.end()) {
        return std::dynamic_pointer_cast<T>(it->second);
    }
    return nullptr;
}

template Shared<Texture> AssetLibrary::GetAsset(AssetHandle handle);
template Shared<Shader> AssetLibrary::GetAsset(AssetHandle handle);
template Shared<Material> AssetLibrary::GetAsset(AssetHandle handle);