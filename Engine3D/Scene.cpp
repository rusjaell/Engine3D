#include "pch.h"
#include "Scene.h"
#include "ShaderLibrary.h"
#include "Event.h"
#include "Application.h"

#include "ModelLibrary.h"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Shared<Shader> modelShader_;

Shared<Model> model_;
Shared<Model> backpack_;

Scene::Scene()
{
    modelShader_ = ShaderLibrary::LoadShader("default", "resources/shaders/default.vert", "resources/shaders/default.frag");

    //model_ = ModelLibrary::LoadModel("sponza", "resources/assets/sponza/sponza.obj");
    model_ = ModelLibrary::LoadModel("backpack", "resources/assets/backpack/backpack.obj");
    backpack_ = ModelLibrary::LoadModel("helmet", "resources/assets/damaged_helmet/DamagedHelmet.gltf");
}

Scene::~Scene()
{
}

void Scene::OnEvent(Event& event)
{
}

void Scene::OnImGuiRender(double time, double dt)
{
}

Camera camera({ 0, 0, 0 });

void Scene::OnUpdate(double time, double dt)
{
    camera.MovementSpeed = 2.5f;

    if (!Input::IsKeyRepeating(KeyCode::F1) && Input::IsKeyDown(KeyCode::F1)) {
        ShaderLibrary::Reload(); // will create a memory leak
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
    glClearColor(0.0, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float fov = glm::radians(camera.Zoom);
    float aspectRatio = SCR_WIDTH / SCR_HEIGHT;
    float nearClip = 0.1f; // Near clipping plane
    float farClip = 100.0f; // Far clipping plane

    // Create the perspective projection matrix
    glm::mat4 projection = glm::perspective(fov, aspectRatio, nearClip, farClip);
    glm::mat4 viewMatrix = camera.GetViewMatrix();

    modelShader_->Use();
    modelShader_->SetUniformMatrix4f("u_Projection", projection);
    modelShader_->SetUniformMatrix4f("u_View", viewMatrix);
    
    glm::mat4 model(1.0);
    model = glm::scale(model, glm::vec3 { 0.00625f });
    modelShader_->SetUniformMatrix4f("u_Model", model);

    model_->Draw(modelShader_);

    model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(5, 0, 0));
    model = glm::scale(model, glm::vec3{ 1 });

    modelShader_->SetUniformMatrix4f("u_Model", model);
    backpack_->Draw(modelShader_);

    //for (int x = 0; x < 8; x++) {
    //    for (int y = 0; y < 8; y++) {
    //        for (int z = 0; z < 8; z++) {

    //            model = glm::mat4(1.0);
    //            model = glm::translate(model, glm::vec3(x - 4, y + 12, z - 4));
    //            //model = glm::scale(model, glm::vec3{ 0.00625f / 4 });
    //            model = glm::scale(model, glm::vec3{ 0.5f / 4 });

    //            modelShader_->SetUniformMatrix4f("u_Model", model);

    //            backpack_->Draw(modelShader_);
    //        }
    //    }
    //}
}


void Scene::GeometryPass()
{
}

void Scene::ShadowPass()
{
}

void Scene::LightingPass()
{
}

void Scene::PostProcessingPass()
{
}