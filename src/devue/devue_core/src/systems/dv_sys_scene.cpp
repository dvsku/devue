#include "systems/dv_sys_scene.hpp"
#include "systems/dv_systems_bundle.hpp"

using namespace devue::core;

dv_sys_scene::dv_sys_scene(dv_systems_bundle* systems) 
	: m_systems(systems) {}

dv_scene* dv_sys_scene::create_scene() {
	try {
		devue::uuid uuid = dv_util_uuid::create();

		m_scenes[uuid] = dv_scene();
		current_scene = &m_scenes[uuid];

		return current_scene;
	}
	catch (...) {
		return nullptr;
	}
}