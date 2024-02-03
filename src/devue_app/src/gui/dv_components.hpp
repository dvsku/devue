#pragma once

#include "gui/components/dv_comp_assets.hpp"
#include "gui/components/dv_comp_console.hpp"
#include "gui/components/dv_comp_dockspace.hpp"
#include "gui/components/dv_comp_hierarchy.hpp"
#include "gui/components/dv_comp_meshes.hpp"
#include "gui/components/dv_comp_properties.hpp"
#include "gui/components/dv_comp_scene.hpp"
#include "gui/components/dv_comp_texture.hpp"
#include "gui/modals/dv_modal_import.hpp"
#include "gui/modals/dv_modal_plugins.hpp"
#include "systems/dv_systems.hpp"

namespace devue {
    struct dv_components {
    	dv_comp_assets	   assets;
    	dv_comp_console	   console;
    	dv_comp_dockspace  dockspace;
    	dv_comp_hierarchy  hierarchy;
    	dv_comp_properties properties;
    	dv_comp_scene	   scene;
        dv_comp_meshes     meshes;
        dv_comp_texture    texture;

        dv_modal_import  modal_import;
        dv_modal_plugins modal_plugins;

        devue::uuid checkerboard_uuid = 0U;

    	dv_components() = delete;
    	dv_components(dv_systems* systems);
    };
}