#include "gui/base/dv_comp.hpp"
#include "gui/dv_components.hpp"

using namespace devue;

dv_comp::dv_comp(dv_systems* systems, dv_components* components)
    : m_systems(systems), m_components(components) {}
