#include "gui/components/dv_comp_assets.hpp"

using namespace devue;
dv_comp_assets::dv_comp_assets(dv_systems* systems)
	: m_systems(systems) {}

void dv_comp_assets::render() {
	ImGui::Begin("Assets##Window");

	/*for (auto& kvp : m_model_system.models) {
		ds_model& model = kvp.second;

		ImGui::PushID(model.guid.to_string().c_str());

		if (ImGui::TreeNodeEx(FORMAT_C("{}##Asset", model.name), ImGuiTreeNodeFlags_Leaf)) {

			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Selectable("Add to scene##ModelPopup", false,
					ImGuiSelectableFlags_SelectOnRelease | ImGuiSelectableFlags_NoSetKeyOwner)) {
					m_scene.add_to_scene(model);
				}

				ImGui::EndPopup();
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}*/

	ImGui::End();
}
