#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x3.hpp"
#include "models/pdo/dv_skeleton.hpp"
#include "models/pdo/dv_mesh.hpp"
#include "models/pdo/dv_material.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>

namespace devue::core {
	struct dv_model {
		devue::uuid uuid = 0U;

		std::string name				  = "";
		std::filesystem::path texture_dir = "";
		
		dv_skeleton skeleton;
		std::unordered_map<devue::uuid, dv_mesh>	 meshes;
		std::unordered_map<devue::uuid, dv_material> materials;

		float min_y = 0.0f;

		dv_model()				  = default;
		dv_model(const dv_model&) = default;
		dv_model(dv_model&&)	  = default;

		dv_model& operator=(const dv_model&) = default;
		dv_model& operator=(dv_model&&)		 = default;
	};
}