#include "systems/dv_sys_shader.hpp"
#include "systems/dv_systems_bundle.hpp"

using namespace devue::core;

dv_sys_shader::dv_sys_shader(dv_systems_bundle* systems)
    : m_systems(systems) {}

bool dv_sys_shader::prepare() {
    return true;
}

void dv_sys_shader::release() {}
