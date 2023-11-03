#include "gui/components/dv_comp_properties.hpp"
#include "gui/components/dv_components.hpp"

using namespace devue;

dv_comp_properties::dv_comp_properties(dv_systems* systems, dv_components* components) 
	: dv_comp(systems, components) {}

void dv_comp_properties::render() {
	ImGui::Begin("Properties##Window");

	switch (m_components->selected.type) {
		case dv_selected::obj_type::camera:				render_camera();			break;
		case dv_selected::obj_type::ambient_light:		render_ambient_light();		break;
		case dv_selected::obj_type::directional_light:	render_directional_light(); break;
		default: break;
	}

	ImGui::End();
}

void dv_comp_properties::render_camera() {
	if (!m_systems->scene.current_scene) return;
	core::dv_camera& camera = m_systems->scene.current_scene->camera;

	ImGui::Text("Position");

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 3);
	ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x);
	ImGui::BeginDisabled();
	ImGui::InputFloat3("##PropertiesCameraPos", (float*)&camera.position, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::EndDisabled();
}

void dv_comp_properties::render_ambient_light() {
	if (!m_systems->scene.current_scene) return;
	core::dv_ambient_light& light = m_systems->scene.current_scene->lighting.ambient_light;

	ImGui::PushID("PropertiesAmbientLight");

	ImGui::Checkbox("##Enabled", &light.is_enabled);

	ImGui::SameLine(0.0f, 5.0f);
	ImGui::Text("Enabled");

	if (!light.is_enabled)
		ImGui::BeginDisabled();

	ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoOptions
		| ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel;

	ImGui::ColorEdit3("Light color##Color", (float*)&light.color, flags);
	ImGui::SameLine(0.0f, 5.0f);
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::SliderFloat("##Intensity", &light.intensity, 0.0f, 1.0f);

	if (!light.is_enabled)
		ImGui::EndDisabled();

	ImGui::PopID();
}

void dv_comp_properties::render_directional_light() {
	if (!m_systems->scene.current_scene) return;
	core::dv_directional_light& light = m_systems->scene.current_scene->lighting.directional_light;

	ImGui::PushID("PropertiesDirectionalLight");

	ImGui::Checkbox("##Enabled", &light.is_enabled);
	
	ImGui::SameLine(0.0f, 5.0f);
	ImGui::Text("Enabled");

	if (!light.is_enabled)
		ImGui::BeginDisabled();

	ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoOptions
		| ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel;

	ImGui::ColorEdit3("Light color##Color", (float*)&light.color, flags);
	ImGui::SameLine(0.0f, 5.0f);
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::SliderFloat("##Intensity", &light.intensity, 0.0f, 1.0f);

	if (!light.is_enabled)
		ImGui::EndDisabled();

	ImGui::PopID();
}
