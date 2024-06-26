#include "gui/components/dv_comp_assets.hpp"
#include "gui/dv_components.hpp"
#include "utilities/dv_util_imgui.hpp"

using namespace devue;
using namespace devue::core;

dv_comp_assets::dv_comp_assets(dv_systems* systems, dv_components* components)
    : dv_comp(systems, components) {}

void dv_comp_assets::render() {
    if (ImGui::Begin("Assets##Window")) {
        for (auto& [uuid, model] : m_systems->model.models) {
            bool is_selected = m_systems->properties.is_inspected(model);
            if (dv_util_imgui::selectable(DV_FORMAT("{}##Asset{}", model.name, model.uuid).c_str(), is_selected)) {
                m_systems->properties.set_inspected(model);
            }

            render_context_menu(model);
        }
    }
    ImGui::End();
}

void dv_comp_assets::render_context_menu(dv_model& model) {
    if (libgui::imgui::begin_item_context_menu()) {
        if (ImGui::IsWindowAppearing()) {
            m_systems->properties.set_inspected(model);
        }

        ImGui::PushID("ContextMenu");

        if (dv_util_imgui::selectable("Add to scene", false, 
            ImGuiSelectableFlags_SelectOnRelease | ImGuiSelectableFlags_NoSetKeyOwner)) 
        {
            m_systems->scene.add_to_scene(model);
        }

        ImGui::Separator();

        if (dv_util_imgui::selectable("Remove", false,
            ImGuiSelectableFlags_SelectOnRelease | ImGuiSelectableFlags_NoSetKeyOwner)) 
        {
            m_systems->model.remove(model);
        }

        ImGui::PopID();
        libgui::imgui::end_item_context_menu();
    }
}
