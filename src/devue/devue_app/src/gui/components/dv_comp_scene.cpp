#include "gui/components/dv_comp_scene.hpp"

using namespace devue;

void dv_comp_scene::render() {
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Scene##Window", 0, window_flags);

	//m_hovered = ImGui::IsWindowHovered();


	ImVec2 available_size = ImGui::GetContentRegionAvail();

	ImVec2 clipped_top_left = { 0.0f, 1.0f };
	ImVec2 clipped_bottom_right = { 1.0f, 0.0f };


	/*if (available_size.x < m_frame_buffer.width || available_size.y < m_frame_buffer.height) {
		clipped_top_left = {
			(0.5f - (available_size.x / 2.0f) / m_frame_buffer.width),
			1.0f - (0.5f - (available_size.y / 2.0f) / m_frame_buffer.height)
		};

		clipped_bottom_right = {
			(0.5f + (available_size.x / 2.0f) / m_frame_buffer.width),
			1.0f - (0.5f + (available_size.y / 2.0f) / m_frame_buffer.height)
		};
	}

	ImGui::Image(
		(ImTextureID)m_frame_buffer.get_frame_texture(),
		ImGui::GetContentRegionAvail(),
		clipped_top_left,
		clipped_bottom_right
	);*/



	/*ImGui::Image(
		(ImTextureID)m_frame_buffer.get_frame_texture(),
		{ static_cast<float>(m_width), static_cast<float>(m_height) },
		ImVec2(0, 1),
		ImVec2(1, 0)
	);*/

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
