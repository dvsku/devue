#pragma once

#include "glm/glm.hpp"

namespace devue::core {
	struct dv_vertex {
		uint8_t bone_id = 0U;

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;

		glm::mat4 transform;
	};
}