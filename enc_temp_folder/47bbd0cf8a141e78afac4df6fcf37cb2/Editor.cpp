#include "pch.h"
#include "Editor.h"

Editor::Editor(const char* title, unsigned int width, unsigned int height)
    : Application(title, width, height), viewportSize_({})
{
    FrameBufferSpecification spec;
    spec.clearColor = glm::vec4(0, 0, 0, 1);
    spec.clearBit = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
    viewportFrameBuffer_ = MakeShared<FrameBuffer>(spec);
    
    scene_ = new Scene();
    scene_->SetViewport(window_->width(), window_->height());

    // theme

    auto& style = ImGui::GetStyle();
    style.ChildRounding = 4;

    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    directoryCache_.InitializeRoot("assets");
    directoryCache_.Print();
}

Editor::~Editor()
{
}

void Editor::OnEvent(Event& event)
{
}

void Editor::OnImGuiRender(double time, double dt)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::ShowStyleEditor(&style);

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

                ImGui::DockBuilderDockWindow("###SceneViewport", dockspace_id);
                ImGui::DockBuilderDockWindow("Hierarchy", hierarchy_node_id);
                ImGui::DockBuilderDockWindow("Inspector", inspector_node_id);
                ImGui::DockBuilderDockWindow("Debug", debug_node_id);
                ImGui::DockBuilderDockWindow("Content", console_node_id);
                ImGui::DockBuilderDockWindow("Console", console_node_id);

                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

        ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0.35));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });

        RenderViewport();

        ImGui::PopStyleVar();

        RenderHierarchy();
        RenderConsole();
        RenderInspector();
        RenderDebug();
        RenderContent();

        ImGui::PopStyleColor();

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

    // Get the animated character based on the time and frame count
    char animationChar = "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3];
    int frameCount = ImGui::GetFrameCount();

    // Create a stringstream to construct the string
    std::stringstream ss;
    ss << "Scene " << animationChar << " " << frameCount << "###SceneViewport";

    ImGui::Begin(ss.str().c_str(), 0);

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

    ImGui::Spacing();

    ImGui::BeginChild("Hierarchy Child", ImVec2(0, 0), ImGuiWindowFlags_::ImGuiWindowFlags_None);

    ImGui::EndChild();

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
    ImGui::Begin("Content", 0);// , ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);

    //// Begin main menu bar
    //if (ImGui::BeginMenuBar())
    //{
    //    if (ImGui::BeginMenu("File"))
    //    {
    //        // Add menu items here
    //        ImGui::EndMenu();
    //    }

    //    // Add more menu items if needed

    //    ImGui::EndMenuBar();
    //}

    ImGui::Columns(2, "ContentColumns");

    ImGui::Text("Folders");

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
    float availableWidth = ImGui::GetContentRegionAvailWidth();
    drawList->AddLine(ImVec2(cursorScreenPos.x, cursorScreenPos.y), ImVec2(cursorScreenPos.x + availableWidth, cursorScreenPos.y), ImGui::GetColorU32(ImGuiCol_::ImGuiCol_Border));
    ImGui::Spacing();

    RenderAssetList();

    ImGui::NextColumn();
    ImGui::Text("Files");

    drawList = ImGui::GetWindowDrawList();
    cursorScreenPos = ImGui::GetCursorScreenPos();
    availableWidth = ImGui::GetContentRegionAvailWidth();
    drawList->AddLine(ImVec2(cursorScreenPos.x, cursorScreenPos.y), ImVec2(cursorScreenPos.x + availableWidth, cursorScreenPos.y), ImGui::GetColorU32(ImGuiCol_::ImGuiCol_Border));
    ImGui::Spacing();

    RenderAssetListContent();

    ImGui::End();

    ImGui::ShowDemoWindow();
}

void Editor::RenderAssetList()
{
    ImGui::BeginChild("##Content List");

    RenderAssetListTreeView(directoryCache_.root);

    ImGui::EndChild();
}

#include <filesystem>

Shared<DirectoryCacheEntry> activeEntry_;

void Editor::RenderAssetListTreeView(const Shared<DirectoryCacheEntry>& entry)
{
    ImGuiStyle& style = ImGui::GetStyle();
    float originalIndentSpacing = style.IndentSpacing;
    style.IndentSpacing = 16;

    for (const Shared<DirectoryCacheEntry>& nextEntry : entry->entries)
    {
        const std::string& filePath = nextEntry->path.string();
        const std::string& fileName = nextEntry->name;

        bool selected = activeEntry_ == nextEntry;

        ImGui::AlignTextToFramePadding();

        int flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_OpenOnArrow;
        if (selected) {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        if (nextEntry->entries.size() == 0){
            flags |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;
        }

        if (ImGui::TreeNodeEx(fileName.c_str(), flags)) {

            if (ImGui::IsItemClicked()) {
                activeEntry_ = nextEntry;
            }

            RenderAssetListTreeView(nextEntry);

            ImGui::TreePop();
        }
        else {
            if (ImGui::IsItemClicked()) {
                activeEntry_ = nextEntry;
            }
        }
    }

    style.IndentSpacing = originalIndentSpacing;
}


void Editor::RenderAssetListContent()
{
    if (activeEntry_ == nullptr) {
        return;
    }

    ImGui::BeginChild("##Content ListContent");

    static float thumbnailSize = 96.0f;
    static float padding = 0.0f;

    ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
    ImGui::SliderFloat("Padding", &padding, 0, 32);

    float cellSize = thumbnailSize + padding;

    float panelWidth = ImGui::GetContentRegionAvailWidth();
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1)
        columnCount = 1;

    ImGui::Columns(columnCount, 0, false);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    for (const Shared<FileEntry>& fileEntry : activeEntry_->files) {

        unsigned int id = viewportFrameBuffer_->colorAttachment();

        ImGui::ImageButton((void*)id, { cellSize, cellSize }, { 0, 1 }, { 1, 0 });

        ImGui::TextWrapped(fileEntry->name.c_str());

        ImGui::NextColumn();
    }
    ImGui::PopStyleColor();

    ImGui::Columns(1);

    ImGui::EndChild();
}

