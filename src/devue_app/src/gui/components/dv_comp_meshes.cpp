#include "gui/components/dv_comp_meshes.hpp"
#include "gui/dv_components.hpp"

using namespace devue;

dv_comp_meshes::dv_comp_meshes(dv_systems* systems, dv_components* components)
    : dv_comp(systems, components) {}

bool dv_comp_meshes::render() {
    ImGui::Begin("Meshes##Window");

    ImGui::End();

    return DV_COMMAND_REPEAT;
}
