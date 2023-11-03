#include "systems/dv_sys_scene.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "utilities/dv_util_log.hpp"

using namespace devue::core;

dv_sys_scene::dv_sys_scene(dv_systems_bundle* systems) 
	: m_systems(systems) {}

dv_scene* dv_sys_scene::create_scene() {
	try {
		devue::uuid uuid = dv_util_uuid::create();

		m_scenes[uuid] = dv_scene();
		current_scene = &m_scenes[uuid];

		m_systems->rendering.prepare_model(current_scene->grid.meshes.back(),
										   current_scene->grid.model.meshes[dv_util_uuid::create("grid")]);

		return current_scene;
	}
	catch (const std::exception& e) {
		DV_LOG("Failed to create scene. | {}", e.what());
		return nullptr;
	}
	catch (...) {
		DV_LOG("Failed to create scene.",);
		return nullptr;
	}
}