#include "systems/dv_sys_properties.hpp"
#include "models/pdo/dv_model.hpp"
#include "scene/model/dv_scene_model.hpp"
#include "scene/lighting/dv_ambient_light.hpp"
#include "scene/lighting/dv_directional_light.hpp"
#include "scene/dv_camera.hpp"

using namespace devue;

dv_sys_properties::dv_sys_properties(dv_systems* systems) 
    : m_systems(systems) 
{
    m_supported_types[typeid(core::dv_model)]             = inspectable::type::model;
    m_supported_types[typeid(core::dv_scene_model)]       = inspectable::type::scene_model;
    m_supported_types[typeid(core::dv_camera)]            = inspectable::type::camera;
    m_supported_types[typeid(core::dv_ambient_light)]     = inspectable::type::ambient_light;
    m_supported_types[typeid(core::dv_directional_light)] = inspectable::type::directional_light;
}

void dv_sys_properties::unset_inspected() {
    inspect();
}

const inspectable& devue::dv_sys_properties::get_inspected() {
    return m_current_inspectable;
}

void dv_sys_properties::inspect(inspectable::type type, libutil::uuid id) {
    m_current_inspectable.inspected_type = type;
    m_current_inspectable.inspected_id   = id;
}
