#pragma once

#include "models/pdo/dv_vertex.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <string>
#include <vector>

namespace devue::core {
    struct dv_mesh {
    	std::string name = "";

    	devue::uuid material_uuid{};

    	std::vector<dv_vertex>	  vertices;
    	std::vector<glm::u16vec3> faces;

    	dv_mesh()				= default;
    	dv_mesh(const dv_mesh&) = default;
    	dv_mesh(dv_mesh&&)		= default;

    	dv_mesh& operator=(const dv_mesh&) = default;
    	dv_mesh& operator=(dv_mesh&&)	   = default;
    };
}