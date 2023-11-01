#pragma once

#include "utilities/dv_util_uuid.hpp"

#include <cstdint>
#include <string>

namespace devue::core {
	struct dv_scene_mesh {
		std::string name = "";

		devue::uuid material_uuid;

		uint32_t vao		= 0U;
		uint32_t vbo		= 0U;
		uint32_t ibo		= 0U;
		uint64_t face_count = 0U;
	};
}