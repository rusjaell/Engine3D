#include "pch.h"
#include "Scene.h"
#include "ShaderLibrary.h"
#include "Event.h"
#include "Application.h"
#include "ModelLibrary.h"

#include "AssetLibrary.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Shared<Shader> modelShader_;

Shared<Model> sponza_;
Shared<Model> backPack_;
Shared<Model> helmet_;
Shared<Model> test_;
Shared<Model> breakfast_;

int modelIndex_;

Shared<Model> activeModel_;

Camera camera({ 0, 3.5, 0 });

bool pressedReload = false;
bool f2Pressed = false;
bool pressed = false;
glm::vec3 backgroundColor = { 0, 0, 0 };

glm::vec4 lightVector = { 0, 0, 0, 1.000 };
glm::vec3 lightDiffuseColor = { 1, 1, 1 };
glm::vec3 lightAmbientColor = { 1, 1, 1 };
glm::vec3 lightSpecularColor = { 1, 1, 1 };

Scene::Scene()
{
    modelShader_ = ShaderLibrary::LoadShader("default", "assets/shaders/default.vert", "assets/shaders/default.frag");

    sponza_ = ModelLibrary::LoadModel("sponza", "assets/models/sponza/sponza.obj");
    sponza_->scale_ = glm::vec3(0.00625f, 0.00625f, 0.00625f);

    backPack_ = ModelLibrary::LoadModel("backpack", "assets/models/backpack/backpack.obj");
    backPack_->scale_ = glm::vec3(0.00625f, 0.00625f, 0.00625f);

    test_ = ModelLibrary::LoadModel("material_test", "assets/models/material_test/material_test.obj");

    helmet_ = ModelLibrary::LoadModel("helmet", "assets/models/damaged_helmet/DamagedHelmet.gltf");

    breakfast_ = ModelLibrary::LoadModel("breakfast", "assets/models/breakfast_room/breakfast_room.obj");

    activeModel_ = sponza_;

    //Shared<Material> m = AssetLibrary::LoadMaterial("pathTest");
    //Shared<Texture> t = AssetLibrary::LoadTexture("pathTest");
    //Shared<Shader> s = AssetLibrary::LoadShader("pathTest");

    //Shared<Material> a = AssetLibrary::GetAsset<Material>(m->assetHandle_);
    //Shared<Texture> b = AssetLibrary::GetAsset<Texture>(t->assetHandle_);
    //Shared<Shader> c = AssetLibrary::GetAsset<Shader>(s->assetHandle_);

    std::cout << '\n';
}

Scene::~Scene()
{
}

void Scene::OnEvent(Event& event)
{
}

void Scene::OnImGuiRender(double time, double dt)
{
    ImGui::Begin("Lights");
    ImGui::DragFloat4("##Lights Position", &lightVector[0], 0.5f, 0, 0, "%.3f");
    ImGui::ColorEdit3("##Lights Diffuse Color", &lightDiffuseColor[0]);
    ImGui::ColorEdit3("##Lights Ambient Color", &lightAmbientColor[0]);
    ImGui::ColorEdit3("##Lights Specular Color", &lightSpecularColor[0]);
    ImGui::End();

    ImGui::Begin("Materials");

    std::unordered_set<unsigned int> keys;
    std::vector<Shared<Material>> materials;

    for (const Shared<Mesh>& mesh : activeModel_->meshes) {
        Shared<Material> material = mesh->material_;
        if (keys.find(material->materialId) != keys.end()) {
            continue;
        }
        keys.insert(material->materialId);
        materials.push_back(material);
    }

    std::vector<std::string> materialNames;

    // Populate material names
    for (const auto& material : materials) {
        materialNames.push_back(material->name);
    }

    // Variable to store the selected material index
    static int selectedMaterialIndex = -1;

    ImGui::Spacing();
    ImGui::Text("Material:");

    // Dropdown menu to select material
    ImGui::SetNextItemWidth(128);
    if (ImGui::BeginCombo("##MaterialCombo", selectedMaterialIndex == -1 ? "Select Material" : materialNames[selectedMaterialIndex].c_str())) {
        for (int i = 0; i < materialNames.size(); ++i) {
            bool isSelected = (selectedMaterialIndex == i);
            if (ImGui::Selectable(materialNames[i].c_str(), isSelected)) {
                selectedMaterialIndex = i;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    if (ImGui::Button("Reload Shaders")) {
        ShaderLibrary::Reload();
    }

    if (selectedMaterialIndex != -1 && selectedMaterialIndex < materials.size()) {
        Shared<Material> selectedMaterial = materials[selectedMaterialIndex];

        unsigned int textureID;

        if (selectedMaterial->diffuseTexture != nullptr) {
            textureID = selectedMaterial->diffuseTexture->id();
            ImGui::Spacing();
            ImGui::Text("Diffuse:");
            ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
        }
        ImGui::SliderFloat3("##Diffuse Albedo", &selectedMaterial->diffuseColor[0], 0, 1);

        if (selectedMaterial->ambientTexture != nullptr) {
            textureID = selectedMaterial->ambientTexture->id();
            ImGui::Spacing();
            ImGui::Text("Ambient:");
            ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
        }
        ImGui::SliderFloat3("##Ambient Ambient", &selectedMaterial->ambientColor[0], 0, 1);

        if (selectedMaterial->specularTexture != nullptr) {
            textureID = selectedMaterial->specularTexture->id();
            ImGui::Spacing();
            ImGui::Text("Specular:");
            ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
        }
        ImGui::SliderFloat3("##Specular Specular", &selectedMaterial->specularColor[0], 0, 1);
        ImGui::SliderFloat("##Specular Shininess", &selectedMaterial->shininess, 0, 1000);

        if (selectedMaterial->normalTexture != nullptr) {
            textureID = selectedMaterial->normalTexture->id();
            ImGui::Spacing();
            ImGui::Text("Normal:");
            ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
        }

        //if (selectedMaterial->alphaTexture != nullptr) {
        //    textureID = selectedMaterial->alphaTexture->id();
        //    ImGui::Spacing();
        //    ImGui::Text("Alpha Texture:");
        //    ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
        //}

        //if (selectedMaterial->displacementTexture != nullptr) {
        //    textureID = selectedMaterial->displacementTexture->id();
        //    ImGui::Spacing();
        //    ImGui::Text("Displacement:");
        //    ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
        //}
    }

    ImGui::End();
}

void Scene::OnUpdate(double time, double dt)
{
    camera.MovementSpeed = 2.5f;

    if (Input::IsKeyDown(KeyCode::F1)) {
        if (!pressedReload) {
            ShaderLibrary::Reload();
            pressedReload = true;
        }
    }

    if (Input::IsKeyUp(KeyCode::F1)) {
        pressedReload = false;
    }

    if (Input::IsKeyDown(KeyCode::F2)) {
        if (!f2Pressed) {

            modelIndex_ = (modelIndex_ + 1) % 5;
            std::cout << "Unknown Model: " << modelIndex_ << '\n';

            switch (modelIndex_) {
            case 0:
                activeModel_ = sponza_;
                break;
            case 1:
                activeModel_ = backPack_;
                break;
            case 2:
                activeModel_ = helmet_;
                break;
            case 3:
                activeModel_ = test_;
                break;
            case 4:
                activeModel_ = breakfast_;
                break;
            }
            f2Pressed = true;
        }
    }

    if (Input::IsKeyUp(KeyCode::F2)) {
        f2Pressed = false;
    }

    if (Input::IsKeyDown(KeyCode::Esc)) {
        if (!pressed) {
            Application::instance().ToggleCursor();
            pressed = true;
        }
    }

    if(Input::IsKeyUp(KeyCode::Esc)) {
        pressed = false;
    }

    if (Input::IsKeyDown(KeyCode::W)) {
        camera.ProcessKeyboard(FORWARD, dt);
    }
    if (Input::IsKeyDown(KeyCode::S)) {
        camera.ProcessKeyboard(BACKWARD, dt);
    }
    if (Input::IsKeyDown(KeyCode::A)) {
        camera.ProcessKeyboard(LEFT, dt);
    }
    if (Input::IsKeyDown(KeyCode::D)) {
        camera.ProcessKeyboard(RIGHT, dt);
    }

    static bool firstMouse = false;
    if (Application::instance().isCursorEnabled()) {
        firstMouse = true;
        return;
    }

    static float lastX = 0.0f;
    static float lastY = 0.0f;
    float xpos = static_cast<float>(Input::mouseX());
    float ypos = static_cast<float>(Input::mouseY());

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

    camera.ProcessMouseScroll(Input::scrollDeltaY());
}

void Scene::OnRender(double time, double dt)
{
    float fov = glm::radians(camera.Zoom);
    float aspectRatio = (float)width_ / height_;
    float nearClip = 0.1f; // Near clipping plane
    float farClip = 100.0f; // Far clipping plane

    // Create the perspective projection matrix
    glm::mat4 projection = glm::perspective(fov, aspectRatio, nearClip, farClip);
    glm::mat4 viewMatrix = camera.GetViewMatrix();

    modelShader_->Use();
    modelShader_->SetUniformMatrix4f("u_Projection", projection);
    modelShader_->SetUniformMatrix4f("u_View", viewMatrix);

    modelShader_->SetUniform4f("u_Light.vector", lightVector);
    modelShader_->SetUniform3f("u_Light.diffuseColor", lightDiffuseColor);
    modelShader_->SetUniform3f("u_Light.ambientColor", lightAmbientColor);
    modelShader_->SetUniform3f("u_Light.specularColor", lightSpecularColor);

    modelShader_->SetUniform3f("u_CameraPos", camera.Position);

    activeModel_->Draw(camera, modelShader_);

    sponza_->scale_ = glm::vec3(1.0 / 65);

    test_->position_ = lightVector;
    test_->Draw(camera, modelShader_);
}

void Scene::SetViewport(unsigned int width, unsigned int height)
{
    width_ = width;
    height_ = height;
}
