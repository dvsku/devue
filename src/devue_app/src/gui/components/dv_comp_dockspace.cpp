#include "gui/components/dv_comp_dockspace.hpp"
#include "gui/dv_components.hpp"

using namespace devue;

dv_comp_dockspace::dv_comp_dockspace(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) {}

bool dv_comp_dockspace::render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGuiID dockspace_id	= ImGui::GetID("DockSpace");

    if (ImGui::DockBuilderGetNode(dockspace_id) == NULL) {
    	ImGuiDockNode* node = nullptr;

    	ImGui::DockBuilderRemoveNode(dockspace_id);
    	ImGui::DockBuilderAddNode(dockspace_id);
    	ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

    	ImGuiID left_sidebar_dock = 0;
    	ImGuiID left_sidebar_dock_down = 0;
    	ImGuiID right_sidebar_dock = 0;
    	ImGuiID scene_dock = 0;
    	ImGuiID scene_dock_down = 0;

    	left_sidebar_dock = 
    		ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.15f, nullptr, &scene_dock);
    	
    	left_sidebar_dock_down = 
    		ImGui::DockBuilderSplitNode(left_sidebar_dock, ImGuiDir_Down, 0.50f, nullptr, &left_sidebar_dock);

    	right_sidebar_dock = 
    		ImGui::DockBuilderSplitNode(scene_dock, ImGuiDir_Right, 0.1765f, nullptr, &scene_dock);

    	scene_dock_down = 
    		ImGui::DockBuilderSplitNode(scene_dock, ImGuiDir_Down, 0.20f, nullptr, &scene_dock);

    	ImGui::DockBuilderDockWindow("Assets##Window", left_sidebar_dock_down);
    	ImGui::DockBuilderDockWindow("Hierarchy##Window", left_sidebar_dock);
    	ImGui::DockBuilderDockWindow("Properties##Window", right_sidebar_dock);
    	ImGui::DockBuilderDockWindow("Console##Window", scene_dock_down);
    	ImGui::DockBuilderDockWindow("Scene##Window", scene_dock);

    	node = ImGui::DockBuilderGetNode(dockspace_id);
    	node->LocalFlags |= ImGuiDockNodeFlags_DockSpace |
    		ImGuiDockNodeFlags_NoTabBar;

    	node = ImGui::DockBuilderGetNode(scene_dock);
    	node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;

    	node = ImGui::DockBuilderGetNode(left_sidebar_dock);
    	node->SharedFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;

    	ImGui::DockBuilderFinish(dockspace_id);
    }

    ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | 
    	ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
    	ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    ImGui::Begin("Root##Window", NULL, flags);
    ImGui::PopStyleVar();

    auto dockspace_size = ImGui::GetContentRegionAvail();
    dockspace_size.y -= 27;

    ImGui::DockSpace(dockspace_id, dockspace_size);

    if (ImGui::BeginMenuBar()) {
    	ImGui::PushID("FileMenu");

    	if (ImGui::BeginMenu("File")) {
    		if (ImGui::MenuItem("Import##MenuItem")) {
                m_systems->command.set_execute(dv_commands::flag_show_modal_import);
    		}

    		ImGui::EndMenu();
    	}

    	ImGui::PopID();

    	ImGui::PushID("ViewMenu");

    	if (ImGui::BeginMenu("View")) {
            bool& is_executable = m_systems->command.is_executable(dv_commands::flag_show_console);
    		if (ImGui::MenuItem("Console##MenuItem", "", &is_executable)) {

    		}

    		ImGui::EndMenu();
    	}

    	ImGui::PopID();

    	ImGui::EndMenuBar();
    }

    ImGui::BeginChild("Footer", ImVec2(-1, 23));

    ImGui::AlignTextToFramePadding();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
    ImGui::Text("M: %d SM: %d MAT: %d TEX: %d",
    			m_systems->model.count(),
    			m_systems->scene.count(),
    			m_systems->material.count(),
                m_systems->texture.count());

    ImGui::EndChild();

    ImGui::End();

    return true;
}
