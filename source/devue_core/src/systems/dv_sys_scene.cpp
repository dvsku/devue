#include "systems/dv_sys_scene.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "glad/glad.h"

using namespace devue::core;

dv_sys_scene::dv_sys_scene(dv_systems_bundle* systems) 
    : m_systems(systems) {}

bool dv_sys_scene::prepare() {
    return create_scene();
}

void dv_sys_scene::release() {
    current_scene = nullptr;

    for (auto& [scene_id, scene] : m_scenes) {
        current_scene = &scene;

        for (auto& [smodel_id, smodel] : scene.models) {
            remove_from_scene(smodel);
        }

        remove_marked_models();

        m_systems->rendering.release_model(scene.grid);
    }

    current_scene = nullptr;
    m_scenes.clear();
}

size_t dv_sys_scene::count() const {
    if (!current_scene) return 0;
    return current_scene->models.size();
}

dv_scene* dv_sys_scene::create_scene() {
    try {
        libutil::uuid uuid = libutil::create_uuid();

    	m_scenes[uuid] = dv_scene();
    	current_scene = &m_scenes[uuid];

    	m_systems->rendering.prepare_model(current_scene->grid, current_scene->grid.model);

    	return current_scene;
    }
    catch (const std::exception& e) {
        DV_LOG_ERRO("", "Failed to create scene. | {}", e.what());
    	return nullptr;
    }
    catch (...) {
        DV_LOG_ERRO("", "Failed to create scene.",);
    	return nullptr;
    }
}

void dv_sys_scene::render_current_scene(dv_render_target* render_target) {
    if (!current_scene) return;

    remove_marked_models();
    
    if (render_target)
    	render_target->bind();
    else
    	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clear scene
    glClearColor(0.185f, 0.185f, 0.185f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the scene
    for (auto& [scene_model_id, scene_model] : current_scene->models)
    	if (scene_model.visible && !scene_model.marked_for_removal)
    		m_systems->rendering.render(scene_model, current_scene->camera, current_scene->lighting);

    m_systems->rendering.render(current_scene->grid, current_scene->camera, current_scene->lighting);

    if (render_target)
    	render_target->unbind();
}

void dv_sys_scene::add_to_scene(dv_model& model) {
    if (!current_scene) return;

    try {
        libutil::uuid uuid			= libutil::create_uuid();
    	current_scene->models[uuid] = dv_scene_model();

    	dv_scene_model& smodel = current_scene->models[uuid];
    	smodel.name		  = model.name;
    	smodel.uuid		  = uuid;
    	smodel.model_uuid = model.uuid;

        // Scale model to fit in scene
        rescale_model(model, smodel);

    	m_systems->rendering.prepare_model(smodel);
    }
    catch (...) {
    	return;
    }
}

void dv_sys_scene::remove_from_scene(dv_scene_model& smodel) {
    if (!current_scene) return;
    if (!current_scene->models.contains(smodel.uuid)) return;

    smodel.marked_for_removal = true;
}

void dv_sys_scene::remove_marked_models() {
    if (!current_scene) return;

    try {
    	auto it = current_scene->models.begin();
    	while (it != current_scene->models.end()) {
    		if (it->second.marked_for_removal) {
    			m_systems->material.release_materials(it->second);
    			m_systems->rendering.release_model(it->second);

    			it = current_scene->models.erase(it);
    		}
    		else {
    			++it;
    		}
    	}
    }
    catch(...) {}
}

void dv_sys_scene::rescale_model(dv_model& model, dv_scene_model& smodel) {
    float scaling = (std::max)({
        model.bounding_box.maximum.x / 8.0f,
        model.bounding_box.maximum.y / 8.0f,
        model.bounding_box.maximum.z / 8.0f
    });

    smodel.transform.scale.x /= scaling;
    smodel.transform.scale.y /= scaling;
    smodel.transform.scale.z /= scaling;
}
