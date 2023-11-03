#pragma once

#include "scene/model/dv_scene_model.hpp"
#include "scene/dv_camera.hpp"
#include "scene/lighting/dv_lighting.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
	class dv_scene {
	public:
		devue::uuid uuid = 0U;

		dv_camera camera;
		dv_lighting lighting;
		std::unordered_map<devue::uuid, dv_scene_model> models;

		dv_scene()					= default;
		dv_scene(const dv_scene&)	= delete;
		dv_scene(dv_scene&&)		= default;

		dv_scene& operator=(const dv_scene&) = delete;
		dv_scene& operator=(dv_scene&&)		 = default;
	};
}