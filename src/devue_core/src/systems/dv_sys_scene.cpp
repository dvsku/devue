#include "systems/dv_sys_scene.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "utilities/dv_util_log.hpp"
#include "glad/glad.h"

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

void dv_sys_scene::render_current_scene(dv_render_target* render_target) {
	if (!current_scene) return;
	
	if (render_target)
		render_target->bind();
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clear scene
	glClearColor(0.185f, 0.185f, 0.185f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the scene
	for (auto& kvp : current_scene->models)
		if (kvp.second.visible)				// TODO: Add remove flag
			m_systems->rendering.render(kvp.second, current_scene->camera, current_scene->lighting);

	m_systems->rendering.render(current_scene->grid, current_scene->camera, current_scene->lighting);

	if (render_target)
		render_target->unbind();
}

void dv_sys_scene::add_to_scene(dv_model& model) {
	if (!current_scene) return;

	try {
		devue::uuid uuid			= dv_util_uuid::create();
		current_scene->models[uuid] = dv_scene_model();

		dv_scene_model& smodel = current_scene->models[uuid];
		smodel.name		  = model.name;
		smodel.uuid		  = uuid;
		smodel.model_uuid = model.uuid;

		if (model.min_y < 0.0f)
			smodel.transform.position.y = abs(model.min_y);

		m_systems->rendering.prepare_model(smodel);
	}
	catch (...) {
		return;
	}
}