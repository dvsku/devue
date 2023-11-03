#pragma once

#include "scene/model/dv_scene_mesh.hpp"
#include "scene/model/dv_transform.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <string>
#include <vector>

namespace devue::core {
	struct dv_scene_model {
		///////////////////////////////////////////////////////////////////////
		// MODEL PROPERTIES

		std::string name = "";
		devue::uuid model_uuid = 0U;

		///////////////////////////////////////////////////////////////////////
		// RENDER PROPERTIES

		dv_transform transform{};
		std::vector<dv_scene_mesh> meshes;
	};
}