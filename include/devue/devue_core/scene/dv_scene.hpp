#pragma once

#include "scene/model/dv_scene_model.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
	class dv_scene {
	public:
		devue::uuid uuid = 0U;
		std::unordered_map<devue::uuid, dv_scene_model> models;

		dv_scene()					= default;
		dv_scene(const dv_scene&)	= delete;
		dv_scene(dv_scene&&)		= default;

		dv_scene& operator=(const dv_scene&) = delete;
		dv_scene& operator=(dv_scene&&)		 = default;
	};
}