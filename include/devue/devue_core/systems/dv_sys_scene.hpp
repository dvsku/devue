#pragma once

#include "scene/dv_scene.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
	struct dv_systems_bundle;

	class dv_sys_scene {
	public:
		dv_sys_scene() = delete;
		dv_sys_scene(dv_systems_bundle* systems);

		dv_sys_scene(const dv_sys_scene&) = delete;
		dv_sys_scene(dv_sys_scene&&)	  = delete;

		dv_sys_scene& operator=(const dv_sys_scene&) = delete;
		dv_sys_scene& operator=(dv_sys_scene&&)		 = delete;

		dv_scene* create_scene();

	private:
		dv_systems_bundle* m_systems	= nullptr;
		dv_scene* m_current_scene		= nullptr;

		std::unordered_map<devue::uuid, dv_scene> m_scenes;
	};
}