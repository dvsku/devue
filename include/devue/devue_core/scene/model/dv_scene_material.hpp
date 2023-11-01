#pragma once

#include "utilities/dv_util_uuid.hpp"

namespace devue::core {
	struct dv_scene_material {
		devue::uuid diffuse_texture_uuid = 0U;
	};
}