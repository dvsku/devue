#pragma once

#include <dv_gui_opengl/dv_gui_opengl.hpp>

#include <cstdint>
#include <string>

namespace devue::core {
    struct dv_scene_mesh {
    	std::string name = "";

        dvsku::uuid mesh_uuid;
    	dvsku::uuid material_uuid;

    	uint32_t ibo		= 0U;
    	uint64_t face_count = 0U;
    };
}