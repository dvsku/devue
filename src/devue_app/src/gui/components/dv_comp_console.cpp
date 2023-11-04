#include "gui/components/dv_comp_console.hpp"
#include "gui/components/dv_components.hpp"

using namespace devue;

dv_comp_console::dv_comp_console(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) {}

void dv_comp_console::render() {
    if (!visible) return;

    if (!ImGui::Begin("Console##Window", &visible)) 
    	return ImGui::End();
    

    ImGui::End();
}
