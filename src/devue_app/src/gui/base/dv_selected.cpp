#include "gui/base/dv_selected.hpp"

#include "models/pdo/dv_model.hpp"
#include "scene/model/dv_scene_model.hpp"
#include "scene/lighting/dv_ambient_light.hpp"
#include "scene/lighting/dv_directional_light.hpp"
#include "scene/dv_camera.hpp"

using namespace devue;

dv_selected::dv_selected() {
	m_selection_types[typeid(core::dv_model)]				= obj_type::model;
	m_selection_types[typeid(core::dv_scene_model)]			= obj_type::scene_model;
	m_selection_types[typeid(core::dv_camera)]				= obj_type::camera;
	m_selection_types[typeid(core::dv_ambient_light)]		= obj_type::ambient_light;
	m_selection_types[typeid(core::dv_directional_light)]	= obj_type::directional_light;
}

void dv_selected::select(obj_type type, devue::uuid uuid) {
	this->type = type;
	this->uuid = uuid;
}
