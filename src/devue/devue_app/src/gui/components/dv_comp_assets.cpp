#include "gui/components/dv_comp_assets.hpp"
#include "gui/components/dv_components.hpp"
#include "utilities/dv_util_string.hpp"

using namespace devue;
using namespace devue::core;

dv_comp_assets::dv_comp_assets(dv_systems* systems, dv_components* components)
	: dv_comp(systems, components) {}

void dv_comp_assets::render() {
	ImGui::Begin("Assets##Window");

	for (auto& kvp : m_systems->model.models) {
		dv_model& model = kvp.second;

		ImGui::PushID(DV_FORMAT_C("{}", model.uuid));

		if (ImGui::Selectable(DV_FORMAT_C("{}##Asset", model.name), m_components->current_model == model.uuid)) {
			m_components->current_model = model.uuid;
		}

		ImGui::PopID();
	}

	ImGui::End();
}
