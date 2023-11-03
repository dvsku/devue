#pragma once

#include "models/pdo/dv_model.hpp"
#include "scene/model/dv_scene_material.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
	class dv_sys_material {
	public:

	public:
		const dv_scene_material* get_material(devue::uuid uuid);

		void prepare_model_materials(dv_model& model);

	private:
		std::unordered_map<devue::uuid, dv_scene_material> m_materials;
	};
}