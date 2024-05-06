#include "gui/components/dv_comp_hierarchy.hpp"
#include "gui/dv_components.hpp"
#include "utilities/dv_util_imgui.hpp"

using namespace devue;
using namespace devue::core;

dv_comp_hierarchy::dv_comp_hierarchy(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) {}

bool dv_comp_hierarchy::render() {
    // Just create the window if there's no
    // active scene
    if (!m_systems->scene.current_scene) {
    	ImGui::Begin("Hierarchy##Window");
    	ImGui::End();
    	return true;
    }

    bool            is_selected = false;
    core::dv_scene* scene       = m_systems->scene.current_scene;

    ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 0.8f);
    
    ImGui::Begin("Hierarchy##Window");

    ImGui::PushID("Hierarchy");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (dv_util_imgui::collapsible("Cameras##Cameras")) {
        is_selected = m_systems->properties.is_inspected(scene->camera);
    	if (dv_util_imgui::selectable("Camera 1##Camera1", is_selected)) {
            m_systems->properties.set_inspected(scene->camera);
    	}
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (dv_util_imgui::collapsible("Lighting##Lighting")) {
        is_selected = m_systems->properties.is_inspected(scene->lighting.ambient_light);
    	if (dv_util_imgui::selectable("Ambient Light##AmbientLight1", is_selected)) {
            m_systems->properties.set_inspected(scene->lighting.ambient_light);
    	}

        is_selected = m_systems->properties.is_inspected(scene->lighting.directional_light);
    	if (dv_util_imgui::selectable("Directional Light##DirectionalLight1", is_selected)) {
            m_systems->properties.set_inspected(scene->lighting.directional_light);
    	}
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (dv_util_imgui::collapsible("Objects##Objects")) {
    	if (m_systems->scene.current_scene) {
    		for (auto& kvp : m_systems->scene.current_scene->models) {
    			dv_scene_model& smodel = kvp.second;

    			ImGui::PushID(DV_FORMAT("{}", smodel.uuid).c_str());

                is_selected = m_systems->properties.is_inspected(smodel);
    			if (dv_util_imgui::selectable(DV_FORMAT("{}##Object", smodel.name).c_str(), is_selected)) {
                    m_systems->properties.set_inspected(smodel);
    			}

                render_scene_model_context_menu(smodel);

    			ImGui::PopID();
    		}
    	}
    }

    ImGui::PopID();

    ImGui::End();

    ImGui::PopStyleVar();

    return true;
}

void dv_comp_hierarchy::render_scene_model_context_menu(dv_scene_model& smodel) {
    if (dv_util_imgui::begin_item_context_menu()) {
        if (ImGui::IsWindowAppearing()) {
            m_systems->properties.set_inspected(smodel);
        }

        ImGui::PushID("ContextMenu");

        if (ImGui::Selectable("Remove from scene", false, 
            ImGuiSelectableFlags_SelectOnRelease | ImGuiSelectableFlags_NoSetKeyOwner)) 
        {
            m_systems->scene.remove_from_scene(smodel);
        }

        ImGui::Separator();

        if (ImGui::Selectable("Properties", false,
            ImGuiSelectableFlags_SelectOnRelease | ImGuiSelectableFlags_NoSetKeyOwner)) 
        {
            m_systems->properties.set_inspected(smodel);
        }

        ImGui::PopID();
        ImGui::EndPopup();
    }
}
