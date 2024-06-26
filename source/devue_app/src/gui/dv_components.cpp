#include "dv_components.hpp"

using namespace devue;

dv_components::dv_components(dv_systems* systems)
    : assets(systems, this),
      console(systems, this),
      dockspace(systems, this),
      hierarchy(systems, this),
      properties(systems, this),
      scene(systems, this), 
      meshes(systems, this),
      texture(systems, this),
      modal_import(systems, this),
      modal_plugins(systems, this),
      modal_about(systems, this)
{}
