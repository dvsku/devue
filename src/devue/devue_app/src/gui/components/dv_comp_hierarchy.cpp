#include "gui/components/dv_comp_hierarchy.hpp"
#include "gui/components/dv_components.hpp"

using namespace devue;

dv_comp_hierarchy::dv_comp_hierarchy(dv_systems* systems, dv_components* components) 
	: dv_comp(systems, components) {}

void dv_comp_hierarchy::render() {
	ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 0.8f);

	ImGui::Begin("Hierarchy##Window");

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Camera##SceneExplorerCamera")) {
		/*ImGui::Indent();

		ImGui::Text("Position");

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x);
		ImGui::BeginDisabled();
		ImGui::InputFloat3("##SceneExplorerCameraPos", (float*)&m_scene.camera.position, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::EndDisabled();

		ImGui::Unindent();*/
	}

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Lighting##SceneExplorerLighting")) {
		/*ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
		ImGui::Checkbox("##SceneExplorerAmbientVisible", &m_scene.lighting.ambient_light.is_active);

		ImGui::SameLine();

		if (ImGui::CollapsingHeader("Ambient light##SceneExplorerAmbientLighting")) {
			ImGui::Indent(35.0f);

			if (!m_scene.lighting.ambient_light.is_active)
				ImGui::BeginDisabled();

			m_scene.lighting.ambient_light.build_gui();

			if (!m_scene.lighting.ambient_light.is_active)
				ImGui::EndDisabled();

			ImGui::Unindent(35.0f);
		}


		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
		ImGui::Checkbox("##SceneExplorerDirectionalVisible", &m_scene.lighting.directional_light.is_active);

		ImGui::SameLine();

		if (ImGui::CollapsingHeader("Directional light##SceneExplorerDirectionalLighting")) {
			ImGui::Indent(35.0f);

			if (!m_scene.lighting.directional_light.is_active)
				ImGui::BeginDisabled();

			m_scene.lighting.directional_light.build_gui();

			if (!m_scene.lighting.directional_light.is_active)
				ImGui::EndDisabled();

			ImGui::Unindent(35.0f);
		}*/
	}

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Models##SceneExplorerModels")) {
		/*for (auto& kvp : m_scene.models) {
			ds_basic_render_model& rmodel = kvp.second;

			std::string guid_str = rmodel.guid.to_string();
			ImGui::PushID(guid_str.c_str());

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
			ImGui::Checkbox("##Visible", &rmodel.visible);

			ImGui::SameLine();

			if (ImGui::CollapsingHeader(FORMAT_C("{}", rmodel.name), &rmodel.flag_alive)) {
				ImGui::Indent(35.0f);

				if (!rmodel.visible)
					ImGui::BeginDisabled();

				ImGui::Text(rmodel.guid.to_string().c_str());

				ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
				ImGui::Checkbox("##Wireframe", &rmodel.wireframe);

				ImGui::SameLine();
				ImGui::Text("Wireframe");

				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::Indent(10.0f);

					ImGui::Text("Translation");
					ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x - ImGui::GetCursorPosX() + 3);
					ImGui::DragFloat3("##Translate",
									  (float*)&rmodel.transform.translation, 0.1f);


					ImGui::Text("Rotation");
					ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x - ImGui::GetCursorPosX() + 3);
					ImGui::DragFloat3("##Rotate",
									  (float*)&rmodel.transform.rotation, 1.0f, -360.0f, 360.0f);

					ImGui::Text("Scale");
					ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x - ImGui::GetCursorPosX() + 3);
					ImGui::DragFloat3("##Scale",
									  (float*)&rmodel.transform.scale, 0.1f, 0.1f, 10.0f);

					ImGui::Unindent();
				}

				if (!rmodel.visible)
					ImGui::EndDisabled();

				ImGui::Unindent();
			}

			ImGui::PopID();
		}*/
	}

	ImGui::End();

	ImGui::PopStyleVar();
}
