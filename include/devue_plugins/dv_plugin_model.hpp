#pragma once

#include "glm/glm.hpp"

#include <string>
#include <vector>

namespace devue::plugins {
    struct dv_plugin_material {
    	std::string name			= "";
    	std::string diffuse_texture = "";
    };

    struct dv_plugin_vertex {
    	glm::vec3 position;
    	glm::vec3 normal;
    	glm::vec2 uv;
    };

    struct dv_plugin_mesh {
    	std::string name = "";

    	dv_plugin_material material;

    	std::vector<dv_plugin_vertex> vertices;
    	std::vector<uint16_t> indices;
    };

    struct dv_plugin_model {
    	std::vector<dv_plugin_mesh> meshes;
    };
}