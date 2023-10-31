#include "gui/components/dv_comp_properties.hpp"
#include "gui/components/dv_components.hpp"

using namespace devue;

dv_comp_properties::dv_comp_properties(dv_systems* systems, dv_components* components) 
	: dv_comp(systems, components) {}

void dv_comp_properties::render() {
	ImGui::Begin("Properties##Window");
	ImGui::End();
}
