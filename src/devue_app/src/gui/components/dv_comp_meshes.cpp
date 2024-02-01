#include "gui/components/dv_comp_meshes.hpp"
#include "gui/dv_components.hpp"
#include "gui/fonts/font_fontawesome_solid.hpp"
#include "utilities/dv_util_imgui.hpp"

using namespace devue;
using namespace devue::core;

dv_comp_meshes::dv_comp_meshes(dv_systems* systems, dv_components* components)
    : dv_comp(systems, components) {}

bool dv_comp_meshes::render() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    ImGui::Begin("Meshes##Window");
    
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f));
    ImGui::BeginChild("##Head", { 0.0f, 19.0f});
    {
        ImGui::Indent(4.0f);

        const bool is_disabled = m_current_mesh_id == 0U;

        if (is_disabled)
            ImGui::BeginDisabled();

        if (dv_util_imgui::icon_button(ICON_FA_CHEVRON_LEFT"##BackToMeshes", { 19.0f, 19.0f })) {
            m_current_mesh_id = 0U;
        }

        if (is_disabled)
            ImGui::EndDisabled();

        ImGui::Unindent(2.0f);
    }
    ImGui::EndChild();
    ImGui::PopStyleColor(1);

    ImGui::Indent(4.0f);
    ImGui::BeginChild("##Content", { ImGui::GetContentRegionAvail().x - 4.0f, ImGui::GetContentRegionAvail().y });
    {
        switch (m_systems->properties.get_inspected().inspected_type) {
            case inspectable::type::model:       render_model();       break;
            case inspectable::type::scene_model: render_scene_model(); break;
            default: m_current_mesh_id = 0U; break;
        }
    }
    ImGui::EndChild();
    ImGui::Unindent(4.0f);

    ImGui::End();
    ImGui::PopStyleVar(1);

    return DV_COMMAND_REPEAT;
}

void dv_comp_meshes::render_model() {
    uuid uuid = m_systems->properties.get_inspected().inspected_id;

    if (!m_systems->model.models.contains(uuid)) return;
    core::dv_model& model = m_systems->model.models[uuid];

    if (m_current_mesh_id && model.meshes.contains(m_current_mesh_id)) {
        return render_mesh(model.meshes[m_current_mesh_id]);
    }
    else {
        return render_mesh_list(model);
    }
}

void dv_comp_meshes::render_scene_model() {
    uuid uuid = m_systems->properties.get_inspected().inspected_id;

    if (!m_systems->scene.current_scene)                        return;
    if (!m_systems->scene.current_scene->models.contains(uuid)) return;
    core::dv_scene_model& smodel = m_systems->scene.current_scene->models[uuid];

    if (!m_systems->model.models.contains(smodel.model_uuid)) return;
    core::dv_model& model = m_systems->model.models[smodel.model_uuid];

    if (m_current_mesh_id && model.meshes.contains(m_current_mesh_id)) {
        core::dv_mesh& mesh = model.meshes[m_current_mesh_id];

        for (auto& smesh : smodel.meshes) {
            if (smesh.mesh_uuid == m_current_mesh_id)
                return render_scene_mesh(smesh, mesh);
        }
    }

    render_mesh_list(model);
}

void dv_comp_meshes::render_mesh_list(core::dv_model& model) {
    
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.0f, 0.5f));
    for (const auto& [mesh_id, mesh] : model.meshes) {

        if (dv_util_imgui::selectable(mesh.name.c_str(), false, 0, { 0.0f, 15.0f})) {
            m_current_mesh_id = mesh_id;
        }
    }
    ImGui::PopStyleVar(1);
}

void dv_comp_meshes::render_mesh(core::dv_mesh& mesh) {
    ImGui::Indent(4.0f);
    {

    }
    ImGui::Unindent(4.0f);
}

void dv_comp_meshes::render_scene_mesh(core::dv_scene_mesh& smesh, core::dv_mesh& mesh) {
    ImGui::Indent(4.0f);
    {

    }
    ImGui::Unindent(4.0f);

    render_mesh(mesh);
}
