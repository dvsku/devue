#pragma once

#include "models/pdo/dv_model.hpp"
#include "scene/model/dv_scene_model.hpp"
#include "scene/model/dv_scene_material.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
    class dv_sys_material {
    public:
    	const dv_scene_material* get_material(devue::uuid uuid);
    	size_t count() const;

    	void prepare_model_materials(dv_model& model);
    	void release_materials(dv_scene_model& smodel);

    private:
    	std::unordered_map<devue::uuid, std::pair<uint16_t, dv_scene_material>> m_materials;
    };
}