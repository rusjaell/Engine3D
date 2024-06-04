#ifndef MODEL_LOADER
#define MODEL_LOADER

#include "Model.h"

#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ModelLibrary
{
public:
    static Shared<Model> LoadModel(const std::string& name, const std::string& path);
    static Shared<Model> GetModel(const std::string& name);

private:
    static void ProcessNode(const std::string& directory, const Shared<Model>& model, aiNode* node, const aiScene* scene);
    static Mesh ProcessMesh(const std::string& directory, const Shared<Model>& model, aiMesh* mesh, const aiScene* scene);
    static Shared<Texture> LoadTexture(const std::string& directory, aiMaterial* material, aiTextureType type, std::string typeName);

private:
    static unsigned int nextMaterialId_;
    static std::map<std::string, Shared<Material>> loadedMaterials_;
    static std::map<std::string, Shared<Texture>> loadedTextures_;
    static std::map<std::string, Shared<Model>> loadedModels_;
};

#endif