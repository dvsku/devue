#include "gui/components/dv_comp_meshes.hpp"
#include "gui/dv_components.hpp"
#include "utilities/dv_util_imgui.hpp"

using namespace devue;
using namespace devue::core;

dv_comp_meshes::dv_comp_meshes(dv_systems* systems, dv_components* components)
    : dv_comp(systems, components) {}

void dv_comp_meshes::render() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

    if (ImGui::Begin("Meshes##Window")) {
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f));
        if (ImGui::BeginChild("##Head", { 0.0f, 19.0f })) {
            ImGui::Indent(4.0f);

            const bool is_disabled = m_current_mesh_id == 0U;

            if (is_disabled)
                ImGui::BeginDisabled();

            if (libgui::imgui::icon_button(ICON_FA_CHEVRON_LEFT"##BackToMeshes", { 19.0f, 19.0f })) {
                m_current_mesh_id = 0U;
            }

            if (is_disabled)
                ImGui::EndDisabled();

            ImGui::Unindent(2.0f);
        }
        ImGui::EndChild();
        ImGui::PopStyleColor(1);

        ImGui::Indent(4.0f);
        if(ImGui::BeginChild("##Content", { ImGui::GetContentRegionAvail().x - 4.0f, ImGui::GetContentRegionAvail().y })) {
            switch (m_systems->properties.get_inspected().inspected_type) {
                case inspectable::type::model:       render_model();       break;
                case inspectable::type::scene_model: render_scene_model(); break;
                default: m_current_mesh_id = 0U; break;
            }
        }
        ImGui::EndChild();
        ImGui::Unindent(4.0f);
    }
    ImGui::End();

    ImGui::PopStyleVar(1);
}

void dv_comp_meshes::render_model() {
    dvsku::uuid uuid = m_systems->properties.get_inspected().inspected_id;

    if (!m_systems->model.models.contains(uuid)) return;
    core::dv_model& model = m_systems->model.models[uuid];

    if (m_current_mesh_id && model.meshes.contains(m_current_mesh_id)) {
        dv_mesh& mesh = model.meshes[m_current_mesh_id];

        core::dv_material* material = nullptr;
        if (model.materials.contains(mesh.material_uuid))
            material = &model.materials[mesh.material_uuid];

        return render_mesh(nullptr, mesh, material);
    }
    else {
        return render_mesh_list(model);
    }
}

void dv_comp_meshes::render_scene_model() {
    dvsku::uuid uuid = m_systems->properties.get_inspected().inspected_id;

    if (!m_systems->scene.current_scene)                        return;
    if (!m_systems->scene.current_scene->models.contains(uuid)) return;
    core::dv_scene_model& smodel = m_systems->scene.current_scene->models[uuid];

    if (!m_systems->model.models.contains(smodel.model_uuid)) return;
    core::dv_model& model = m_systems->model.models[smodel.model_uuid];

    if (m_current_mesh_id && model.meshes.contains(m_current_mesh_id)) {
        core::dv_mesh& mesh = model.meshes[m_current_mesh_id];

        core::dv_scene_mesh* smesh = nullptr;
        for (auto& sm : smodel.meshes) {
            if (sm.mesh_uuid == m_current_mesh_id) {
                smesh = &sm;
                break;
            }
        }

        core::dv_material* material = nullptr;
        if (model.materials.contains(mesh.material_uuid))
            material = &model.materials[mesh.material_uuid];

        return render_mesh(smesh, mesh, material);
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

void dv_comp_meshes::render_mesh(core::dv_scene_mesh* smesh, core::dv_mesh& mesh, core::dv_material* material) {
    ImGui::Indent(4.0f);
    {
        const float indent = 8.0f;

        ImGui::SeparatorText("General");
        ImGui::Text("Faces: %d", mesh.faces.size());

        if (material) {
            const dv_scene_material* smaterial = nullptr;
            const dv_scene_texture*  stexture  = nullptr;

            if (smesh && smesh->material_uuid)
                smaterial = m_systems->material.get_material(smesh->material_uuid);

            ImGui::SeparatorText("Material");
            
            ImGui::TextWrapped("Name: %s", !material->name.empty() ? material->name.c_str() : "No name");
            ImGui::Dummy({ 0.0f, 3.0f });

            if (smaterial && smaterial->diffuse_texture_uuid)
                stexture = m_systems->texture.get_texture(smaterial->diffuse_texture_uuid);

            if (stexture && stexture->texture_id) {
                ImGui::Image((void*)(intptr_t)stexture->texture_id, { 45.0f, 45.0f }, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));

                if (ImGui::IsItemClicked()) {
                    m_components->texture.set_texture(smaterial->diffuse_texture_uuid, material->diffuse_texture);
                    m_systems->command.set_to_execute(dv_commands::flag_show_texture);
                    m_components->texture.focus();
                }

                ImGui::SameLine(0.0f, 8.0f);
            }

            ImGui::TextWrapped("Diffuse texture:\n  %s", !material->diffuse_texture.empty() ? material->diffuse_texture.c_str() : "No texture");
        }
    }
    ImGui::Unindent(4.0f);
}
