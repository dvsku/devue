#pragma once

#include "scene/lighting/dv_light.hpp"

namespace devue::core {
    struct dv_directional_light : public dv_light {
    	glm::ivec3 direction = { 0, -1, -1 };

    	dv_directional_light() : dv_light() {}
    };
}