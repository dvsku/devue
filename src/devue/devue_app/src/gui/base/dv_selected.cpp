#include "gui/base/dv_selected.hpp"

#include "models/pdo/dv_model.hpp"
#include "scene/model/dv_scene_model.hpp"
#include "scene/dv_camera.hpp"

using namespace devue;

dv_selected::dv_selected() {
	m_selection_types[typeid(core::dv_model)]		= obj_type::model;
	m_selection_types[typeid(core::dv_scene_model)] = obj_type::scene_model;
	m_selection_types[typeid(core::dv_camera)]		= obj_type::camera;
}

void dv_selected::select(obj_type type, devue::uuid uuid) {}
