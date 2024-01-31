#include "dv_components.hpp"

using namespace devue;

dv_components::dv_components(dv_systems* systems)
    : assets(systems, this), console(systems, this), dockspace(systems, this),
      hierarchy(systems, this), properties(systems, this), scene(systems, this),
      modal_import(systems), modal_plugins(systems) {}
