#include "gui/modals/dv_modal_about.hpp"
#include "imgui.h"
#include "imgui_internal.h"

using namespace devue;

dv_modal_about::dv_modal_about(dv_systems* systems) 
    : m_systems(systems) {}

bool dv_modal_about::render() {
    return DV_COMMAND_REPEAT;
}
