#pragma once

#include "models/dv_bounding_box.hpp"
#include "models/pdo/dv_skeleton.hpp"
#include "models/pdo/dv_mesh.hpp"
#include "models/pdo/dv_material.hpp"
#include "models/pdo/dv_vertex.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>

namespace devue::core {
    struct dv_model {
    	devue::uuid uuid = 0U;

    	std::string name				  = "";
    	std::filesystem::path texture_dir = "";
    	
    	dv_skeleton                                  skeleton;
        std::vector<dv_vertex>	                     vertices;
    	std::unordered_map<devue::uuid, dv_mesh>     meshes;
    	std::unordered_map<devue::uuid, dv_material> materials;

        dv_bounding_box bounding_box;

        bool flag_z_up_to_y_up = false;
        bool flag_lh_to_rh     = false;

        bool marked_for_removal = false;

    	dv_model()				  = default;
    	dv_model(const dv_model&) = default;
    	dv_model(dv_model&&)	  = default;

    	dv_model& operator=(const dv_model&) = default;
    	dv_model& operator=(dv_model&&)		 = default;
    };
}