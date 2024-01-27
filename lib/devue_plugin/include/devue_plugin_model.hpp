#pragma once

#include "glm/glm.hpp"

#include <string>
#include <vector>

namespace devue::plugins {
    struct devue_plugin_material {
    	std::string name			= "";
    	std::string diffuse_texture = "";
    };

    struct devue_plugin_vertex {
    	glm::vec3 position;
    	glm::vec3 normal;
    	glm::vec2 uv;
    };

    struct devue_plugin_mesh {
    	std::string                      name = "";
        devue_plugin_material            material;
    	std::vector<devue_plugin_vertex> vertices;
    	std::vector<uint16_t>            indices;
    };

    struct devue_plugin_model {
    	std::vector<devue_plugin_mesh> meshes;
    };
}