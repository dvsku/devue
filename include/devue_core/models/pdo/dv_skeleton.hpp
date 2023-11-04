#pragma once

#include <string>
#include <vector>

namespace devue::core {
    struct dv_bone {
    	std::string name = "";
    	std::string parent = "";

    	std::vector<glm::mat4x3> frame_transforms;

    	dv_bone()				= default;
    	dv_bone(const dv_bone&) = default;
    	dv_bone(dv_bone&&)		= default;

    	dv_bone& operator=(const dv_bone&) = default;
    	dv_bone& operator=(dv_bone&&)	   = default;
    };

    struct dv_animation {
    	std::string name = "";

    	uint32_t frame_start = 0U;
    	uint32_t frame_end	 = 0U;
    };

    struct dv_skeleton {
    	std::vector<dv_bone>	  bones;
    	std::vector<dv_animation> animations;

    	dv_skeleton()					= default;
    	dv_skeleton(const dv_skeleton&) = default;
    	dv_skeleton(dv_skeleton&&)		= default;

    	dv_skeleton& operator=(const dv_skeleton&) = default;
    	dv_skeleton& operator=(dv_skeleton&&)	   = default;
    };
}