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

    ImGui::Columns(2, "ContentColumns");

    RenderAssetList(); // Column 1
    RenderAssetListContent(); // Column 2
    
    ImGui::Columns(1);

    ImGui::End();

    ImGui::ShowDemoWindow();
}

void DrawRowsBackground(int row_count, float line_height, float x1, float x2, float y_offset, ImU32 col_even, ImU32 col_odd)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float y0 = ImGui::GetCursorScreenPos().y + (float)(int)y_offset;

    int row_display_start;
    int row_display_end;
    ImGui::CalcListClipping(row_count, line_height, &row_display_start, &row_display_end);
    for (int row_n = row_display_start; row_n < row_display_end; row_n++)
    {
        ImU32 col = (row_n & 1) ? col_odd : col_even;
        if ((col & IM_COL32_A_MASK) == 0) {
            continue;
        }

        float y1 = y0 + (line_height * row_n);
        float y2 = y1 + line_height;
        draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col);
    }

    const auto pos = ImGui::GetCursorPos();
    ImGuiListClipper clipper;
    clipper.Begin(row_count, line_height);
    while (clipper.Step())
    {
        for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; ++row_n)
        {
            ImU32 col = (row_n & 1) ? col_odd : col_even;
            if ((col & IM_COL32_A_MASK) == 0) {
                continue;
            }

            float y1 = y0 + (line_height * static_cast<float>(row_n));
            float y2 = y1 + line_height;
            draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col);
        }
    }
}

void Editor::RenderAssetList()
{
    ImGui::BeginChild("##Content List");

    //ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
    RenderAssetListTreeView("assets");
    //ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

    ImGui::EndChild();
}

#include <filesystem>

namespace fs = std::filesystem;

std::string selectedDirectory_;

#include "imgui_internal.h" // for RenderArrow()

bool MyCollapsingHeader(const char* label)
{
    using namespace ImGui;
    PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
    bool* p_open = GetStateStorage()->GetBoolRef(GetID(label), false);
    if (Button(label, ImVec2(-FLT_MIN, 0.0f)))
        *p_open ^= 1;
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 arrow_pos = ImVec2(GetItemRectMax().x - style.FramePadding.x - GetFontSize(), GetItemRectMin().y + style.FramePadding.y);
    RenderArrow(GetWindowDrawList(), arrow_pos, GetColorU32(ImGuiCol_Text), *p_open ? ImGuiDir_Down : ImGuiDir_Right);
    PopStyleVar();
    return *p_open;
}

void Editor::RenderAssetListTreeView(const std::string& directory) {


    ImGuiStyle& style = ImGui::GetStyle();
    float originalIndentSpacing = style.IndentSpacing; // Store the original indent spacing
    style.IndentSpacing = 16; // Set your desired indent size

    // Your ImGui rendering code here

    // Restore the original indent spacing after rendering

    for (const auto& entry : fs::directory_iterator(directory))
    {
        const fs::path& entryPath = entry.path();

        const std::string& filePath = entryPath.string();
        const std::string& fileName = entryPath.filename().string();

        bool selected = selectedDirectory_ == filePath;

        if (fs::is_directory(entry))
        {
            ImGui::AlignTextToFramePadding();

            int flags = ImGuiTreeNodeFlags_SpanFullWidth;
            if (selected) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }

            //unsigned int id = viewportFrameBuffer_->colorAttachment();
            //ImGui::Image((void*)id, ImVec2(16, 16), ImVec2{0, 1}, ImVec2{1, 0});
            //ImGui::SameLine();

            if (ImGui::TreeNodeEx(fileName.c_str(), flags)) {
                if (ImGui::IsItemClicked()) {
                    selectedDirectory_ = filePath;
                }
                RenderAssetListTreeView(filePath);
                ImGui::TreePop();
            }
        }
    }

    style.IndentSpacing = originalIndentSpacing;
}


void Editor::RenderAssetListContent()
{
    ImGui::NextColumn();

    ImGui::Text("Column 2 - Row 1");
    ImGui::Text("Column 2 - Row 2");
    ImGui::Text("Column 2 - Row 3");
}
