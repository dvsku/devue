#include "gui/components/dv_comp_dockspace.hpp"
#include "gui/components/dv_components.hpp"

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
		ImGuiID right_sidebar_dock = 0;
		ImGuiID scene_dock = 0;

		left_sidebar_dock = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.15f, nullptr, &scene_dock);
		right_sidebar_dock = ImGui::DockBuilderSplitNode(scene_dock, ImGuiDir_Right, 0.1765f, nullptr, &scene_dock);

		ImGui::DockBuilderDockWindow("Assets##Window", left_sidebar_dock);
		ImGui::DockBuilderDockWindow("Hierarchy##Window", left_sidebar_dock);
		ImGui::DockBuilderDockWindow("Properties##Window", right_sidebar_dock);
		ImGui::DockBuilderDockWindow("Console##Window", right_sidebar_dock);
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
				/*std::string path = "";
				ds_util_dialog::open_file_dialog(path);

				if (!path.empty())
					m_model_system.import(path);*/
			}

			ImGui::EndMenu();
		}

		ImGui::PopID();

		ImGui::PushID("ViewMenu");

		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Console##MenuItem", "", &m_components->console.visible)) {

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
	/*ImGui::Text("M: %d RM: %d MAT: %d", m_model_system.models.size(),
				m_scene.models.size(), m_material_system.get().size());*/

	ImGui::EndChild();

	ImGui::End();
}
