#pragma once

#include "glm/vec3.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <string>
#include <vector>

namespace devue::core {
    struct dv_mesh {
    	std::string name = "";

        devue::uuid uuid;
    	devue::uuid material_uuid{};
    	std::vector<glm::u32vec3> faces;

    	dv_mesh()				= default;
    	dv_mesh(const dv_mesh&) = default;
    	dv_mesh(dv_mesh&&)		= default;

    	dv_mesh& operator=(const dv_mesh&) = default;
    	dv_mesh& operator=(dv_mesh&&)	   = default;
    };
}