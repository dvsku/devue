#include "gui/components/dv_comp_assets.hpp"
#include "gui/dv_components.hpp"
#include "utilities/dv_util_string.hpp"

using namespace devue;
using namespace devue::core;

dv_comp_assets::dv_comp_assets(dv_systems* systems, dv_components* components)
    : dv_comp(systems, components) {}

bool dv_comp_assets::render() {
    ImGui::Begin("Assets##Window");

    for (auto& kvp : m_systems->model.models) {
    	dv_model& model = kvp.second;

    	ImGui::PushID(DV_FORMAT_C("{}", model.uuid));

        bool is_selected = m_systems->properties.is_inspected(model);
    	if (ImGui::Selectable(DV_FORMAT_C("{}##Asset", model.name), is_selected)) {
            m_systems->properties.set_inspected(model);
    	}

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::Selectable("Add to scene##ContextMenu", false,
                ImGuiSelectableFlags_SelectOnRelease | ImGuiSelectableFlags_NoSetKeyOwner)) 
            {
                m_systems->scene.add_to_scene(model);
            }

            ImGui::EndPopup();
        }

    	ImGui::PopID();
    }

    ImGui::End();

    return DV_COMMAND_REPEAT;
}
