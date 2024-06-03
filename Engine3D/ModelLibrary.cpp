#include "pch.h"
#include "ModelLibrary.h"

std::vector<Shared<Texture>> ModelLibrary::loadedTextures_;
std::map<std::string, Shared<Model>> ModelLibrary::loadedModels_;

Shared<Model> ModelLibrary::GetModel(const std::string& name)
{
    if (loadedModels_.find(name) == loadedModels_.end()) {
        std::cout << "Model with name: " << name << " has not been loaded\n";
        return nullptr;
    }
    return loadedModels_[name];
}

Shared<Model> ModelLibrary::LoadModel(const std::string& name, const std::string& path)
{
    loadedTextures_.clear();

    if (loadedModels_.find(name) != loadedModels_.end()) {
        std::cout << "Model with name: " << name << " has already been loaded\n";
        return nullptr;
    }

    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate); //| aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Failed to load model: " << name << " ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return nullptr;
    }

    Shared<Model> model = MakeShared<Model>();

    ProcessNode(path.substr(0, path.find_last_of('/')), model, scene->mRootNode, scene);

    loadedModels_.insert(std::make_pair(name, model));
    return model;
}


void ModelLibrary::ProcessNode(const std::string& directory, const Shared<Model>& model, aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes.push_back(ProcessMesh(directory, model, mesh, scene));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(directory, model, node->mChildren[i], scene);
    }
}

Mesh ModelLibrary::ProcessMesh(const std::string& directory, const Shared<Model>& model, aiMesh* mesh, const aiScene* scene)
{
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indices;
    std::vector<Shared<Texture>> textures;

    // walk through each of the mesh's vertices
    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        VertexData vertex;

        vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

        // uv
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vertex.uv = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

            //// tangent
            //vector.x = mesh->mTangents[i].x;
            //vector.y = mesh->mTangents[i].y;
            //vector.z = mesh->mTangents[i].z;
            //vertex.Tangent = vector;

            //// bitangent
            //vector.x = mesh->mBitangents[i].x;
            //vector.y = mesh->mBitangents[i].y;
            //vector.z = mesh->mBitangents[i].z;
            //vertex.Bitangent = vector;
        }
        else
            vertex.uv = glm::vec2(0.0f, 0.0f);

        // normals
        if (mesh->HasNormals()) {
            vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        }

        vertices.push_back(vertex);
    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        // retrieve all indices of the face and store them in the indices vector
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    Shared<Material> m = MakeShared<Material>();
    if (scene->HasMaterials()) {

        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::cout << '\n';
        int count = material->GetTextureCount(aiTextureType_DIFFUSE);
        std::cout << "aiTextureType_DIFFUSE: " << count << '\n';

        count = material->GetTextureCount(aiTextureType_SPECULAR);
        std::cout << "aiTextureType_SPECULAR: " << count << '\n';

        count = material->GetTextureCount(aiTextureType_AMBIENT);
        std::cout << "aiTextureType_AMBIENT: " << count << '\n';

        count = material->GetTextureCount(aiTextureType_EMISSIVE);
        std::cout << "aiTextureType_EMISSIVE: " << count << '\n';

        count = material->GetTextureCount(aiTextureType_HEIGHT);
        std::cout << "aiTextureType_HEIGHT: " << count << '\n';

        count = material->GetTextureCount(aiTextureType_NORMALS);
        std::cout << "aiTextureType_NORMALS: " << count << '\n';

        count = material->GetTextureCount(aiTextureType_SHININESS);
        std::cout << "aiTextureType_SHININESS: " << count << '\n';

        count = material->GetTextureCount(aiTextureType_OPACITY);
        std::cout << "aiTextureType_OPACITY: " << count << '\n';

        count = material->GetTextureCount(aiTextureType_DISPLACEMENT);
        std::cout << "aiTextureType_DISPLACEMENT: " << count << '\n';

        count = material->GetTextureCount(aiTextureType_UNKNOWN);
        std::cout << "aiTextureType_UNKNOWN: " << count << '\n';

        // Albedo map (diffuse texture)
        aiString path;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
            m->albedoMap = MakeShared<Texture>(directory + "/" + path.C_Str());
        }

        // Roughness map
        if (material->GetTexture(aiTextureType_SHININESS, 0, &path) == AI_SUCCESS) {
            m->roughnessMap = MakeShared<Texture>(directory + "/" + path.C_Str());
        }

        // Normal map
        if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS) {
            m->normalMap = MakeShared<Texture>(directory + "/" + path.C_Str());
        }

        // Ambient occlusion map
        if (material->GetTexture(aiTextureType_AMBIENT, 0, &path) == AI_SUCCESS) {
            m->ambientOcclusionMap = MakeShared<Texture>(directory + "/" + path.C_Str());
        }

        // Specular map
        if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
            m->specularMap = MakeShared<Texture>(directory + "/" + path.C_Str());
        }

        // Metallic map
        if (material->GetTexture(aiTextureType_METALNESS, 0, &path) == AI_SUCCESS) {
            m->metallicMap = MakeShared<Texture>(directory + "/" + path.C_Str());
        }

        aiColor4D diffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
        aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
        m->defaultAlbedoColor = { diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a };

        aiColor4D ambientColor(1.0f, 1.0f, 1.0f, 1.0f);
        aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
        m->defaultAmbientColor = { ambientColor.r, ambientColor.g, ambientColor.b, ambientColor.a };
    }
    else {
        std::cout << "No Material " << '\n';
        assert(false);
    }

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, m);
}

std::vector<Shared<Texture>> ModelLibrary::LoadMaterialTextures(const std::string& directory, aiMaterial* mat, aiTextureType type, std::string typeName)
{
    return {};
}
