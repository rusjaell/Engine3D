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

        // 1. diffuse maps
        std::vector<Shared<Texture>> diffuseMaps = LoadMaterialTextures(directory, material, aiTextureType_DIFFUSE, "u_Albedo");
        if (diffuseMaps.size() > 1) {
            std::cout << "MULTIPLE DIFFUSES\n";
        }
        else  if (diffuseMaps.size() == 0) {
        }
        else {
            m->diffuseMap = diffuseMaps[0];
        }

        // 2. specular maps
        std::vector<Shared<Texture>> specularMaps = LoadMaterialTextures(directory, material, aiTextureType_SPECULAR, "u_Specular");
        if (specularMaps.size() > 1) {
            std::cout << "MULTIPLE SPECULAR\n";
        }
        else  if (specularMaps.size() == 0){
        }
        else {
            m->specularMap = specularMaps[0];
        }

        // 3. normal maps
        std::vector<Shared<Texture>> normalMaps = LoadMaterialTextures(directory, material, aiTextureType_HEIGHT, "u_Normal");
        if (normalMaps.size() > 1) {
            std::cout << "MULTIPLE NORMAL\n";
        }
        else  if (normalMaps.size() == 0) {
        }
        else {
            m->normalMap = normalMaps[0];
        }

        // 4. height maps
        std::vector<Shared<Texture>> ambientHeightMaps = LoadMaterialTextures(directory, material, aiTextureType_AMBIENT, "u_Ambient");
        if (ambientHeightMaps.size() > 1) {
            std::cout << "MULTIPLE AMBIENT HEIGHT\n";
        }
        else  if (ambientHeightMaps.size() == 0) {
        }
        else {
            m->ambientHeightMap = ambientHeightMaps[0];
        }

        if (aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &m->shininess) != AI_SUCCESS) 
        {
            m->shininess = 0.0f;
        }

        aiColor4D diffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
        aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
        m->defaultDiffuseColor = { diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a };

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
    std::vector<Shared<Texture>> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {

        aiString str;
        mat->GetTexture(type, i, &str);

        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < loadedTextures_.size(); j++) {
            if (std::strcmp(loadedTextures_[j]->type().data(), str.C_Str()) == 0) {
                textures.push_back(loadedTextures_[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            std::string path = directory + "/" + str.C_Str();

            Shared<Texture> texture = MakeShared<Texture>(typeName, path);
            textures.push_back(texture);
            loadedTextures_.push_back(texture);
        }
    }
    return textures;
}
