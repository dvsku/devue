#pragma once

#include "systems/dv_sys_model.hpp"
#include "systems/dv_sys_material.hpp"
#include "systems/dv_sys_texture.hpp"
#include "systems/dv_sys_rendering.hpp"
#include "systems/dv_sys_scene.hpp"
#include "systems/dv_sys_plugin.hpp"
#include "systems/dv_sys_shader.hpp"

namespace devue::core {
    struct dv_systems_bundle {
    	dv_sys_model     model;
    	dv_sys_material  material;
    	dv_sys_texture   texture;
    	dv_sys_rendering rendering;
    	dv_sys_scene     scene;
        dv_sys_plugin    plugin;
        dv_sys_shader    shader;

    	dv_systems_bundle();
    };
}