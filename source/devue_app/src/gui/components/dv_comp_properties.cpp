#include "gui/components/dv_comp_properties.hpp"
#include "gui/dv_components.hpp"
#include "utilities/dv_util_imgui.hpp"

using namespace devue;

dv_comp_properties::dv_comp_properties(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) {}

void dv_comp_properties::render() {
    if (ImGui::Begin("Properties##Window")) {
        switch (m_systems->properties.get_inspected().inspected_type) {
            case inspectable::type::model:             render_model();             break;
            case inspectable::type::scene_model:	   render_scene_model();       break;
            case inspectable::type::camera:			   render_camera();            break;
            case inspectable::type::ambient_light:	   render_ambient_light();     break;
            case inspectable::type::directional_light: render_directional_light(); break;
            default: break;
        }
    }
    ImGui::End();
}

void dv_comp_properties::render_model() {
    libutil::uuid uuid = m_systems->properties.get_inspected().inspected_id;

    if (!m_systems->model.models.contains(uuid)) return;
    core::dv_model& model = m_systems->model.models[uuid];

    ImGui::PushID("PropertiesModel");

    ImGui::Checkbox("Flip Z-up to Y-up##ZupToYup", &model.flag_z_up_to_y_up);
    ImGui::SameLine();
    dv_util_imgui::help_marker("Flip model from Z axis up to Y axis up");
    
    ImGui::Checkbox("Flip LH to RH##LHToRH", &model.flag_lh_to_rh);
    ImGui::SameLine();
    dv_util_imgui::help_marker("Flip model 180 degrees around up axis");

    ImGui::PopID();
}

void dv_comp_properties::render_scene_model() {
    libutil::uuid uuid = m_systems->properties.get_inspected().inspected_id;
    
    if (!m_systems->scene.current_scene)                        return;
    if (!m_systems->scene.current_scene->models.contains(uuid)) return;

    core::dv_scene_model& smodel = m_systems->scene.current_scene->models[uuid];

    ImGui::PushID("PropertiesSceneModel");

    ImGui::Checkbox("##Enabled", &smodel.visible);

    ImGui::SameLine(0.0f, 5.0f);
    ImGui::Text("Visible");

    if (!smodel.visible)
    	ImGui::BeginDisabled();

    ImGui::Checkbox("##Wireframe", &smodel.wireframe);

    ImGui::SameLine(0.0f, 5.0f);
    ImGui::Text("Wireframe");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Transform##Transform")) {
    	ImGui::Indent(10.0f);

    	ImGui::Text("Position");
    	ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x - ImGui::GetCursorPosX() + 3);
    	ImGui::DragFloat3("##Position",
    					  (float*)&smodel.transform.position, 0.1f);


    	ImGui::Text("Rotation");
    	ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x - ImGui::GetCursorPosX() + 3);
    	ImGui::DragFloat3("##Rotation",
    					  (float*)&smodel.transform.rotation, 1.0f, -360.0f, 360.0f);

    	ImGui::Text("Scale");
    	ImGui::SetNextItemWidth(ImGui::GetContentRegionMax().x - ImGui::GetCursorPosX() + 3);
    	ImGui::DragFloat3("##Scale",
    					  (float*)&smodel.transform.scale, 0.1f, 0.1f, 10.0f);

    	ImGui::Unindent();
    }

    if (!smodel.visible)
    	ImGui::EndDisabled();

    ImGui::PopID();
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
