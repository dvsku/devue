#pragma once

#include "glm/vec3.hpp"

#include <dv_gui_opengl/dv_gui_opengl.hpp>
#include <string>
#include <vector>

namespace devue::core {
    struct dv_mesh {
    	std::string name          = "";
        dvsku::uuid uuid          = 0U;
    	dvsku::uuid material_uuid = 0U;

    	std::vector<glm::u32vec3> faces;

    	dv_mesh()				= default;
    	dv_mesh(const dv_mesh&) = default;
    	dv_mesh(dv_mesh&&)		= default;

    	dv_mesh& operator=(const dv_mesh&) = default;
    	dv_mesh& operator=(dv_mesh&&)	   = default;
    };
}