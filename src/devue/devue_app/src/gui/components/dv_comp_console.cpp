#include "gui/components/dv_comp_console.hpp"

using namespace devue;

void dv_comp_console::render() {
	if (!visible) return;

	if (!ImGui::Begin("Console##Window", &visible)) 
		return ImGui::End();
	

	ImGui::End();
}
