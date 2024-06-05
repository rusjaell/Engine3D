#include "pch.h"
#include "ModelLibrary.h"

unsigned int ModelLibrary::nextMaterialId_ = 0;
std::map<std::string, Shared<Material>> ModelLibrary::loadedMaterials_;
std::map<std::string, Shared<Texture>> ModelLibrary::loadedTextures_;
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
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices); //| aiProcess_FlipUVs);
    //const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate);
    //const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Failed to load model: " << name << " ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return nullptr;
    }

    std::cout << "Scene Name: " << scene->mName.C_Str() << std::endl;

    Shared<Model> model = MakeShared<Model>();

    ProcessNode(path.substr(0, path.find_last_of('/')), model, scene->mRootNode, scene, scene->mRootNode->mTransformation);

    loadedModels_.insert(std::make_pair(name, model));
    return model;
}


void ModelLibrary::ProcessNode(const std::string& directory, const Shared<Model>& model, aiNode* node, const aiScene* scene, const aiMatrix4x4& parentTransform)
{  
    // Compute the node's global transformation
    aiMatrix4x4 globalTransform = parentTransform * node->mTransformation;

    // Decompose the transformation matrix into position, rotation, and scale
    aiVector3D position;
    aiQuaternion rotation;
    aiVector3D scale;
    
    globalTransform.Decompose(scale, rotation, position);

    //std::cout << "Node Name: " << node->mName.C_Str() << std::endl;
    //std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    //std::cout << "Rotation: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ", " << rotation.w << ")" << std::endl;
    //std::cout << "Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
    //std::cout << std::endl;

    model->position_ = glm::vec3(position.x, position.y, position.z);
    model->rotation_ = glm::quat(glm::radians(glm::vec3(rotation.x, rotation.y, rotation.z)));
    model->scale_ = glm::vec3(scale.x, scale.y, scale.z);
    
    if (scene->HasMeshes()) {
        for (int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            model->meshes.push_back(ProcessMesh(directory, model, mesh, scene));
        }
    }
    else {
        std::cout << "No meshes found\n";
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(directory, model, node->mChildren[i], scene, globalTransform);
    }
}

Shared<Mesh> ModelLibrary::ProcessMesh(const std::string& directory, const Shared<Model>& model, aiMesh* mesh, const aiScene* scene)
{
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indices;
    std::vector<Shared<Texture>> textures;

    for (int i = 0; i < mesh->mNumVertices; i++)
    {
        VertexData vertex;

        vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };

        // uv
        if (mesh->mTextureCoords[0])
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

    for (int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    Shared<Material> m;
    if (scene->HasMaterials()) 
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        int shadingModel;
        material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);

        //if (shadingModel != aiShadingMode_Phong && shadingModel != aiShadingMode_Gouraud)
        //{
        //    std::cout << "Invalid ShadingModel: " << shadingModel << " setting to default material\n";
        //    m->name= "DefaultMaterial";
        //}
        //else {

        //}
        //
        std::string materialKey = material->GetName().C_Str();
        if (loadedMaterials_.find(materialKey) != loadedMaterials_.end())
        {
            m = loadedMaterials_[materialKey];
        }
        else
        {
            m = MakeShared<Material>();
            m->name = materialKey;
            m->materialId = nextMaterialId_;
            nextMaterialId_++;

            //std::cout << '\n';
            //int count = material->GetTextureCount(aiTextureType_DIFFUSE);
            //std::cout << "aiTextureType_DIFFUSE: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_SPECULAR);
            //std::cout << "aiTextureType_SPECULAR: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_AMBIENT);
            //std::cout << "aiTextureType_AMBIENT: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_EMISSIVE);
            //std::cout << "aiTextureType_EMISSIVE: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_HEIGHT);
            //std::cout << "aiTextureType_HEIGHT: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_NORMALS);
            //std::cout << "aiTextureType_NORMALS: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_SHININESS);
            //std::cout << "aiTextureType_SHININESS: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_OPACITY);
            //std::cout << "aiTextureType_OPACITY: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_DISPLACEMENT);
            //std::cout << "aiTextureType_DISPLACEMENT: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_LIGHTMAP);
            //std::cout << "aiTextureType_LIGHTMAP: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_REFLECTION);
            //std::cout << "aiTextureType_REFLECTION: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_BASE_COLOR);
            //std::cout << "aiTextureType_BASE_COLOR: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_NORMAL_CAMERA);
            //std::cout << "aiTextureType_NORMAL_CAMERA: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_EMISSION_COLOR);
            //std::cout << "aiTextureType_EMISSION_COLOR: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_METALNESS);
            //std::cout << "aiTextureType_METALNESS: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_DIFFUSE_ROUGHNESS);
            //std::cout << "aiTextureType_DIFFUSE_ROUGHNESS: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_AMBIENT_OCCLUSION);
            //std::cout << "aiTextureType_AMBIENT_OCCLUSION: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_SHEEN);
            //std::cout << "aiTextureType_SHEEN: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_CLEARCOAT);
            //std::cout << "aiTextureType_CLEARCOAT: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_TRANSMISSION);
            //std::cout << "aiTextureType_TRANSMISSION: " << count << '\n';

            //count = material->GetTextureCount(aiTextureType_UNKNOWN);
            //std::cout << "aiTextureType_UNKNOWN: " << count << '\n';

            aiString path;
        
            m->diffuseTexture = LoadTexture(directory, material, aiTextureType_DIFFUSE, "DIFFUSE");
            m->ambientTexture = LoadTexture(directory, material, aiTextureType_AMBIENT, "AMBIENT");
            m->specularTexture = LoadTexture(directory, material, aiTextureType_SPECULAR, "SPECULAR");

            //m->normalTexture = LoadTexture(directory, material, aiTextureType_NORMALS, "NORMALS");
            //m->alphaTexture = LoadTexture(directory, material, aiTextureType_OPACITY, "ALPHA");
            //m->displacementTexture = LoadTexture(directory, material, aiTextureType_DISPLACEMENT, "DISPLACEMENT");

            aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);
            aiColor3D ambientColor(0.0f, 0.0f, 0.0f);
            aiColor3D specularColor(0.0f, 0.0f, 0.0f);
            float shininess = 0.0f;

            material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
            material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);
            material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
            material->Get(AI_MATKEY_SHININESS, shininess);

            m->diffuseColor = { diffuseColor.r, diffuseColor.g, diffuseColor.b };
            m->ambientColor = { ambientColor.r, ambientColor.g, ambientColor.b };
            m->specularColor = { specularColor.r, specularColor.g, specularColor.b };
            m->shininess = shininess;

            if (m->shininess == 0.0)
                m->shininess = 30;

            std::cout << "Loaded Material: " << materialKey << " set to id: " << m->materialId << '\n';

            loadedMaterials_.insert(std::make_pair(materialKey, m));
        }
    }
    else {
        std::cout << "No Material " << '\n';
        assert(false);
    }
    
    return MakeShared<Mesh>(vertices, indices, m);
}

Shared<Texture> ModelLibrary::LoadTexture(const std::string& directory, aiMaterial* material, aiTextureType type, std::string typeName)
{
    std::string key;

    aiString path;
    aiReturn result = material->GetTexture(type, 0, &path);
    if (result != aiReturn_SUCCESS) {

        if (type == aiTextureType::aiTextureType_DIFFUSE) {
            key = "default_diffuse_texture";
            if (loadedTextures_.find(key) == loadedTextures_.end()) {
                std::cout << "Created Default Diffuse Texture: " << key << '\n';

                Shared<Texture> texture = MakeShared<Texture>(1, 1);

                unsigned int data = 0xFFFFFFFFF;
                texture->SetDataRGBA(&data, sizeof(unsigned int));

                loadedTextures_.insert(std::make_pair(key, texture));
            }
            return loadedTextures_[key];
        }
        return nullptr;
    }

    key = directory + "/" + path.C_Str();
    if (loadedTextures_.find(key) == loadedTextures_.end()) {
        std::cout << "Loaded Texture: " << key << '\n';
        loadedTextures_.insert(std::make_pair(key, MakeShared<Texture>(key)));
    }
    return loadedTextures_[key];
}
