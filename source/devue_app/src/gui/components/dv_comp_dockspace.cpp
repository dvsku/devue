#include "gui/components/dv_comp_dockspace.hpp"
#include "gui/dv_components.hpp"

using namespace devue;

dv_comp_dockspace::dv_comp_dockspace(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) {}

void dv_comp_dockspace::render() {
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
        ImGuiID right_sidebar_dock_down = 0;
    	ImGuiID scene_dock = 0;
    	ImGuiID scene_dock_down = 0;

    	left_sidebar_dock = 
    		ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.15f, nullptr, &scene_dock);
    	
    	left_sidebar_dock_down = 
    		ImGui::DockBuilderSplitNode(left_sidebar_dock, ImGuiDir_Down, 0.50f, nullptr, &left_sidebar_dock);

    	right_sidebar_dock = 
    		ImGui::DockBuilderSplitNode(scene_dock, ImGuiDir_Right, 0.1765f, nullptr, &scene_dock);

        right_sidebar_dock_down =
            ImGui::DockBuilderSplitNode(right_sidebar_dock, ImGuiDir_Down, 0.5f, nullptr, &right_sidebar_dock);

    	scene_dock_down = 
    		ImGui::DockBuilderSplitNode(scene_dock, ImGuiDir_Down, 0.20f, nullptr, &scene_dock);

    	ImGui::DockBuilderDockWindow("Assets##Window", left_sidebar_dock_down);
    	ImGui::DockBuilderDockWindow("Hierarchy##Window", left_sidebar_dock);
    	ImGui::DockBuilderDockWindow("Properties##Window", right_sidebar_dock);
    	ImGui::DockBuilderDockWindow("Console##Window", scene_dock_down);
    	ImGui::DockBuilderDockWindow("Scene##Window", scene_dock);
        ImGui::DockBuilderDockWindow("Meshes##Window", right_sidebar_dock_down);
        ImGui::DockBuilderDockWindow("###TextureWindow", scene_dock);

    	node = ImGui::DockBuilderGetNode(dockspace_id);
    	node->LocalFlags |= ImGuiDockNodeFlags_DockSpace |
    		ImGuiDockNodeFlags_NoTabBar;

    	node = ImGui::DockBuilderGetNode(scene_dock);
    	node->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;

    	node = ImGui::DockBuilderGetNode(left_sidebar_dock);
    	node->SharedFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;

    	ImGui::DockBuilderFinish(dockspace_id);
    }

    auto dockspace_size = ImGui::GetContentRegionAvail();
    dockspace_size.y -= 23;

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
    ImGui::DockSpace(dockspace_id, dockspace_size);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, 0xFF1F1F1F);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 4);
    ImGui::BeginChild("Footer", ImVec2(-1, 27));

    ImGui::AlignTextToFramePadding();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
    ImGui::Text("M: %d SM: %d MAT: %d TEX: %d",
    			m_systems->model.models.size(),
    			m_systems->scene.current_scene ? m_systems->scene.current_scene->models.size() : 0U,
    			m_systems->material.materials.size(),
                m_systems->texture.textures.size());

    ImGui::EndChild();

    ImGui::PopStyleColor();
}
