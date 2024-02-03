#include "systems/dv_sys_material.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "exceptions/dv_exception.hpp"

using namespace devue::core;

dv_sys_material::dv_sys_material(dv_systems_bundle* systems) 
    : m_systems(systems) {}

const dv_scene_material* dv_sys_material::get_material(devue::uuid uuid) {
    if (!materials.contains(uuid)) return nullptr;
    return &materials[uuid].second;
}

void dv_sys_material::prepare_model_materials(dv_model& model) {
    for (auto& [uuid, material] : model.materials) {
    	// Increase ref count and continue
    	if (materials.contains(uuid)) {
    		materials[uuid].first++;
    		continue;
    	}

    	try {
    		materials[uuid] = {
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
    	if (!materials.contains(smesh.material_uuid)) continue;
    	auto& [ref, material] = materials[smesh.material_uuid];

    	if (ref > 1) {
    		ref--;
    		continue;
    	}

        m_systems->texture.release_material_textures(material);
    	materials.erase(smesh.material_uuid);
    }
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

dv_scene_material dv_sys_material::create_scene_material(dv_model& model, dv_material& material) {
    // If material has no textures, don't create it
    if (material.diffuse_texture.empty())
        throw dv_exception("");

    dv_scene_material smaterial;
    m_systems->texture.prepare_material_textures(model, material, smaterial);
    return smaterial;
}