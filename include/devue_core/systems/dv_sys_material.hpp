#pragma once

#include "models/pdo/dv_model.hpp"
#include "scene/model/dv_scene_model.hpp"
#include "scene/model/dv_scene_material.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_material {
    public:
        dv_sys_material() = delete;
        dv_sys_material(dv_systems_bundle* systems);

        dv_sys_material(const dv_sys_material&) = delete;
        dv_sys_material(dv_sys_material&&)      = delete;

        dv_sys_material& operator=(const dv_sys_material&) = delete;
        dv_sys_material& operator=(dv_sys_material&&)      = delete;

    	const dv_scene_material* get_material(devue::uuid uuid);
    	size_t count() const;

    	void prepare_model_materials(dv_model& model);
    	void release_materials(dv_scene_model& smodel);

    private:
        dv_systems_bundle* m_systems;
    	std::unordered_map<devue::uuid, std::pair<uint16_t, dv_scene_material>> m_materials;

    private:
        dv_scene_material create_scene_material(dv_model& model, dv_material& material);
    };
}