#include "systems/dv_systems_bundle.hpp"

using namespace devue::core;

dv_systems_bundle::dv_systems_bundle() 
    : scene(this),
      rendering(this),
      material(this),
      plugin(this),
      model(this),
      texture(this),
      shader(this)
{}