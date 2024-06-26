#pragma once

#include "scene/model/dv_scene_mesh.hpp"
#include "scene/model/dv_transform.hpp"

#include <libutil.hpp>
#include <string>
#include <vector>

namespace devue::core {
    struct dv_scene_model {
    	///////////////////////////////////////////////////////////////////////
    	// MODEL PROPERTIES

    	std::string name = "";

        libutil::uuid uuid	   = 0U;
        libutil::uuid model_uuid = 0U;

        uint32_t vao = 0U;
        uint32_t vbo = 0U;

    	///////////////////////////////////////////////////////////////////////
    	// RENDER PROPERTIES

    	bool visible			= true;
    	bool wireframe			= false;
    	bool marked_for_removal = false;

    	dv_transform transform{};
    	std::vector<dv_scene_mesh> meshes;
    };
}