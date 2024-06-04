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

Shared<Model> model_;
Shared<Model> test_;
Shared<Model> backpack_;

Camera camera({ 0, 0, 0 });

bool pressedReload = false;
bool vSyncPressed = false;
bool pressed = false;
glm::vec3 backgroundColor = { 0, 0, 0 };

Scene::Scene()
{
    modelShader_ = ShaderLibrary::LoadShader("default", "resources/shaders/default.vert", "resources/shaders/default.frag");

    model_ = ModelLibrary::LoadModel("sponza", "resources/assets/sponza/sponza.obj");
    test_ = ModelLibrary::LoadModel("material_test", "resources/asBsets/material_test/material_test.obj");
    backpack_ = ModelLibrary::LoadModel("backpack", "resources/assets/backpack/backpack.obj");
    //backpack_ = ModelLibrary::LoadModel("helmet", "resources/assets/damaged_helmet/DamagedHelmet.gltf");

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
    ImGui::Begin("Environment");
    ImGui::Text("FPS: %d UPS: %d DRAW_CALLS: %d VERTICES: %d", Application::instance().frames(), Application::instance().updates(), Application::instance().draws_, Application::instance().vertices);
    ImGui::SliderFloat3("Background Color", &backgroundColor[0], 0.0, 1.0f);
    //std::cout << "FPS: " << lastFPS_ << " UPS: " << lastUPS_ << " DRAWS: " << draws_ << " VERTICES: " << vertices << '\n';

    ImGui::End();

    ImGui::Begin("Materials");

    std::unordered_set<unsigned int> keys;
    std::vector<Shared<Material>> materials;

    for (size_t i = 0; i < model_->meshes.size(); i++)
    {
        Shared<Material> material = model_->meshes[i].material_;
        if (keys.find(material->materialId) != keys.end()) {
            continue;
        }
        keys.insert(material->materialId);
        materials.push_back(model_->meshes[i].material_);
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
            ImGui::SliderFloat3("##Diffuse Albedo", &selectedMaterial->diffuseColor[0], 0, 1);
        }

        if (selectedMaterial->ambientTexture != nullptr) {
            textureID = selectedMaterial->ambientTexture->id();
            ImGui::Spacing();
            ImGui::Text("Ambient:");
            ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
            ImGui::SliderFloat3("##Ambient Ambient", &selectedMaterial->ambientColor[0], 0, 1);
        }

        if (selectedMaterial->specularTexture != nullptr) {
            textureID = selectedMaterial->specularTexture->id();
            ImGui::Spacing();
            ImGui::Text("Specular:");
            ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
            ImGui::SliderFloat3("##Specular Specular", &selectedMaterial->specularColor[0], 0, 1);
        }

        if (selectedMaterial->normalTexture != nullptr) {
            textureID = selectedMaterial->normalTexture->id();
            ImGui::Spacing();
            ImGui::Text("Normal:");
            ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
        }

        if (selectedMaterial->alphaTexture != nullptr) {
            textureID = selectedMaterial->alphaTexture->id();
            ImGui::Spacing();
            ImGui::Text("Alpha Texture:");
            ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
        }

        if (selectedMaterial->displacementTexture != nullptr) {
            textureID = selectedMaterial->displacementTexture->id();
            ImGui::Spacing();
            ImGui::Text("Displacement:");
            ImGui::Image((ImTextureID)textureID, ImVec2(256, 256));
        }
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
        if (!vSyncPressed) {
            bool vSync = Application::instance().vSync();
            Application::instance().SetVSync(!vSync);
            vSyncPressed = true;
        }
    }

    if (Input::IsKeyUp(KeyCode::F2)) {
        vSyncPressed = false;
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
    //glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    
    glm::mat4 model(1.0);

    glm::vec3 pos(0, 0, 0);

    model = glm::translate(model, glm::vec3(0, 0, 0));
    model = glm::scale(model, glm::vec3{ 0.00625f });
    modelShader_->SetUniformMatrix4f("u_Model", model);

    model_->Draw(modelShader_);

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            for (int z = 0; z < 8; z++) {

                glm::vec3 pos(x - 4, y + 12, z - 4);

                model = glm::mat4(1.0);
                model = glm::translate(model, pos);
                model = glm::scale(model, glm::vec3{ 0.00625f / 4 });
                //model = glm::scale(model, glm::vec3{ 0.5f / 4 });

                modelShader_->SetUniformMatrix4f("u_Model", model);

                backpack_->Draw(modelShader_);
            }
        }
    }
}

void Scene::SetViewport(unsigned int width, unsigned int height)
{
    width_ = width;
    height_ = height;
}
