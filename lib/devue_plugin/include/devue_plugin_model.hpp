#pragma once

#include "devue_plugin_vertex.hpp"
#include "devue_plugin_mesh.hpp"
#include "devue_plugin_material.hpp"

namespace devue::plugins {
    struct devue_plugin_model {
        std::vector<devue_plugin_vertex>   vertices;
    	std::vector<devue_plugin_mesh>     meshes;
        std::vector<devue_plugin_material> materials;
    };
}