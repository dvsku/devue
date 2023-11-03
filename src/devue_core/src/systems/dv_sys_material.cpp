#include "systems/dv_sys_material.hpp"

using namespace devue::core;

const dv_scene_material* dv_sys_material::get_material(devue::uuid uuid) {
	if (!m_materials.contains(uuid)) return nullptr;
	return &m_materials[uuid];
}

void dv_sys_material::prepare_model_materials(dv_model& model) {

}