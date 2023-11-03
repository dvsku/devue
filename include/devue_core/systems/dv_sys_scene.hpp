#pragma once

#include "scene/dv_scene.hpp"
#include "rendering/dv_render_target.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
	struct dv_systems_bundle;

	class dv_sys_scene {
	public:
		dv_scene* current_scene = nullptr;

		dv_sys_scene() = delete;
		dv_sys_scene(dv_systems_bundle* systems);

		dv_sys_scene(const dv_sys_scene&) = delete;
		dv_sys_scene(dv_sys_scene&&)	  = delete;

		dv_sys_scene& operator=(const dv_sys_scene&) = delete;
		dv_sys_scene& operator=(dv_sys_scene&&)		 = delete;

		dv_scene* create_scene();
		void render_current_scene(dv_render_target* render_target = nullptr);

	private:
		dv_systems_bundle* m_systems = nullptr;
		std::unordered_map<devue::uuid, dv_scene> m_scenes;
	};
}