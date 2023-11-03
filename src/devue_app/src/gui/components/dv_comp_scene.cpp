#include "gui/components/dv_comp_scene.hpp"
#include "gui/components/dv_components.hpp"

using namespace devue;

dv_comp_scene::dv_comp_scene(dv_systems* systems, dv_components* components) 
	: dv_comp(systems, components) {}

void dv_comp_scene::render(core::dv_render_target* render_target) {
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Scene##Window", 0, window_flags);

	is_hovered = ImGui::IsWindowHovered();

	if (render_target) {
		ImVec2 available_size = ImGui::GetContentRegionAvail();

		ImVec2 clipped_top_left		= { 0.0f, 1.0f };
		ImVec2 clipped_bottom_right = { 1.0f, 0.0f };

		if (available_size.x < render_target->width || available_size.y < render_target->height) {
			clipped_top_left = {
				(0.5f - (available_size.x / 2.0f) / render_target->width),
				1.0f - (0.5f - (available_size.y / 2.0f) / render_target->height)
			};

			clipped_bottom_right = {
				(0.5f + (available_size.x / 2.0f) / render_target->width),
				1.0f - (0.5f + (available_size.y / 2.0f) / render_target->height)
			};
		}

		ImGui::Image(
			(ImTextureID)render_target->get_frame_texture(),
			ImGui::GetContentRegionAvail(),
			clipped_top_left,
			clipped_bottom_right
		);
	}

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 28);

	/*ImGui::BeginChild("SceneInfo", ImVec2(-1, 23));

	ImGui::AlignTextToFramePadding();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
	ImGui::Text("FPS: %d", ds_util_diag::fps);

	ImGui::EndChild();*/

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::End();

	ImGui::PopStyleVar();
}

void dv_comp_scene::render() {}
