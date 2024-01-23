#include "gui/components/dv_comp_hierarchy.hpp"
#include "gui/components/dv_components.hpp"
#include "utilities/dv_util_string.hpp"

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

    core::dv_scene* scene = m_systems->scene.current_scene;

    ImGui::PushStyleVar(ImGuiStyleVar_DisabledAlpha, 0.8f);

    ImGui::Begin("Hierarchy##Window");

    ImGui::PushID("Hierarchy");

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Cameras##Cameras")) {
    	ImGui::Indent();

    	if (ImGui::Selectable("Camera 1##Camera1", m_components->selected.in_selected(scene->camera))) {
    		m_components->selected.select(scene->camera);
    	}

    	ImGui::Unindent();
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Lighting##Lighting")) {
    	ImGui::Indent();

    	if (ImGui::Selectable("Ambient Light##AmbientLight1", 
    		m_components->selected.in_selected(scene->lighting.ambient_light))) 
    	{
    		m_components->selected.select(scene->lighting.ambient_light);
    	}

    	if (ImGui::Selectable("Directional Light##DirectionalLight1",
    		m_components->selected.in_selected(scene->lighting.directional_light))) 
    	{
    		m_components->selected.select(scene->lighting.directional_light);
    	}

    	ImGui::Unindent();
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::CollapsingHeader("Objects##Objects")) {
    	ImGui::Indent();

    	if (m_systems->scene.current_scene) {
    		for (auto& kvp : m_systems->scene.current_scene->models) {
    			dv_scene_model& smodel = kvp.second;

    			ImGui::PushID(DV_FORMAT_C("{}", smodel.uuid));

    			if (ImGui::Selectable(DV_FORMAT_C("{}##Object", smodel.name),
    				m_components->selected.in_selected(smodel)))
    			{
    				m_components->selected.select(smodel);
    			}

    			if (ImGui::BeginPopupContextItem()) {
    				if (ImGui::Selectable("Remove##ContextMenu", false,
    					ImGuiSelectableFlags_SelectOnRelease | ImGuiSelectableFlags_NoSetKeyOwner)) {
    					m_systems->scene.remove_from_scene(smodel);
    				}

    				ImGui::EndPopup();
    			}

    			ImGui::PopID();
    		}
    	}

    	ImGui::Unindent();
    }

    ImGui::PopID();

    ImGui::End();

    ImGui::PopStyleVar();

    return true;
}
