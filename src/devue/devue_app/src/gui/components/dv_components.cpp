#include "dv_components.hpp"

using namespace devue;

dv_components::dv_components(dv_systems* systems)
	: dockspace(this), assets(systems) {}
