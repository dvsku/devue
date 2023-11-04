#include "systems/dv_sys_material.hpp"

using namespace devue::core;

dv_scene_material create_scene_material(dv_model& model, dv_material& material);

const dv_scene_material* dv_sys_material::get_material(devue::uuid uuid) {
    if (!m_materials.contains(uuid)) return nullptr;
    return &m_materials[uuid].second;
}

size_t dv_sys_material::count() const {
    return m_materials.size();
}

void dv_sys_material::prepare_model_materials(dv_model& model) {
    for (auto& [uuid, material] : model.materials) {
    	// Increase ref count and continue
    	if (m_materials.contains(uuid)) {
    		m_materials[uuid].first++;
    		continue;
    	}

    	try {
    		m_materials[uuid] = {
    			1U,
    			create_scene_material(model, material)
    		};
    	}
    	catch (...) {
    		continue;
    	}
    }
}

void dv_sys_material::release_materials(dv_scene_model& smodel) {
    for (auto& smesh : smodel.meshes) {
    	if (!m_materials.contains(smesh.material_uuid)) continue;
    	auto& [ref, material] = m_materials[smesh.material_uuid];

    	if (ref > 1) {
    		ref--;
    		continue;
    	}

    	// TODO: Remove textures
    	m_materials.erase(smesh.material_uuid);
    }
}

dv_scene_material create_scene_material(dv_model& model, dv_material& material) {
    dv_scene_material scene_material;

    // TODO: Create textures

    return scene_material;
}