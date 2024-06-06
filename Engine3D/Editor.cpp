#include "pch.h"
#include "Editor.h"
#include "Application.h"
#include "Scene.h"

Editor::Editor()
{
    Application& app = Application::instance();

    FrameBufferSpecification spec;
    spec.clearColor = glm::vec4(0, 0, 0, 1);
    spec.clearBit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    viewportFrameBuffer_ = MakeShared<FrameBuffer>(spec);
    
    scene_ = new Scene();
    scene_->SetViewport(app.window().width(), app.window().height());
}

Editor::~Editor()
{
}

void Editor::OnEvent(Event& event)
{
}

void Editor::OnImGuiRender(double time, double dt)
{
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static bool first_time = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) {
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    // Important: note that we proceed even if BeginDockSpace() returns NULL (which is the case if
    // no layout is active).
    if (dockspaceOpen || opt_fullscreen)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen) {
            ImGui::PopStyleVar(2);
        }

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            if (first_time)
            {
                first_time = false;

                Application& app = Application::instance();

                ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(app.window().width(), app.window().height()));

                ImGuiID hierarchy_node_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, NULL, &dockspace_id);
                ImGuiID inspector_node_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, NULL, &dockspace_id);
                ImGuiID debug_node_id = ImGui::DockBuilderSplitNode(inspector_node_id, ImGuiDir_Down, 0.25f, NULL, &inspector_node_id);
                ImGuiID console_node_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, NULL, &dockspace_id);

                ImGui::DockBuilderDockWindow("Viewport", dockspace_id);
                ImGui::DockBuilderDockWindow("Hierarchy", hierarchy_node_id);
                ImGui::DockBuilderDockWindow("Inspector", inspector_node_id);
                ImGui::DockBuilderDockWindow("Debug", debug_node_id);
                ImGui::DockBuilderDockWindow("Content", console_node_id);
                ImGui::DockBuilderDockWindow("Console", console_node_id);

                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

        RenderViewport();

        ImGui::PopStyleVar();

        RenderHierarchy();
        RenderConsole();
        RenderInspector();
        RenderDebug();
        RenderContent();

        ImGui::End();
    }

    scene_->OnImGuiRender(time, dt);
}

void Editor::OnUpdate(double time, double dt)
{
    scene_->OnUpdate(time, dt);
}

void Editor::OnRender(double time, double dt)
{
    FrameBufferSpecification spec = viewportFrameBuffer_->specification();
    if (viewportSize_.x > 0.0f && viewportSize_.y > 0.0f && (spec.width != viewportSize_.x || spec.height != viewportSize_.y)) {
        viewportFrameBuffer_->Resize(viewportSize_.x, viewportSize_.y);
        scene_->SetViewport(viewportSize_.x, viewportSize_.y);
    }

    RenderScene(time, dt);
}

void Editor::RenderScene(double time, double dt)
{
    viewportFrameBuffer_->Bind();
    scene_->OnRender(time, dt);
    viewportFrameBuffer_->Unbind();
}

void Editor::RenderViewport()
{
    ImGuiWindowClass winClass;
    winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;

    ImGui::SetNextWindowClass(&winClass);
    ImGui::Begin("Viewport", 0);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    viewportSize_ = { viewportPanelSize.x, viewportPanelSize.y };

    unsigned int id = viewportFrameBuffer_->colorAttachment();
    ImGui::Image((void*)id, ImVec2{ viewportSize_.x, viewportSize_.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

    ImGui::End();
}

void Editor::RenderHierarchy()
{
    ImGuiWindowClass winClass;
    winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;

    ImGui::SetNextWindowClass(&winClass);
    ImGui::Begin("Hierarchy", 0);

    ImGui::End();
}

void Editor::RenderConsole()
{
    ImGuiWindowClass winClass;
    winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;

    ImGui::SetNextWindowClass(&winClass);
    ImGui::Begin("Console", 0);

    ImGui::End();
}

void Editor::RenderInspector()
{
    ImGuiWindowClass winClass;
    winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;

    ImGui::SetNextWindowClass(&winClass);
    ImGui::Begin("Inspector", 0);
   
    ImGui::End();
}

void Editor::RenderDebug()
{
    ImGuiWindowClass winClass;
    winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;

    ImGui::SetNextWindowClass(&winClass);
    ImGui::Begin("Debug", 0);

    ImGui::Text("FPS: %d, UPS: %d", Application::instance().frames(), Application::instance().updates());
    ImGui::Text("Draws: %d", Application::instance().draws_);
    ImGui::Text("Vertices: %d", Application::instance().vertices);
    ImGui::Text("Triangles: %d", Application::instance().vertices / 3);

    ImGui::Spacing();

    FrameBufferSpecification& spec = viewportFrameBuffer_->specification();

    ImGui::ColorEdit4("ClearColor", &spec.clearColor[0]);
    
    ImGui::End();
}

void Editor::RenderContent()
{
    ImGuiWindowClass winClass;
    winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoWindowMenuButton;

    ImGui::SetNextWindowClass(&winClass);
    ImGui::Begin("Content", 0);

    ImGui::End();
}
