#pragma once

#include "scene/lighting/dv_light.hpp"

namespace devue::core {
    struct dv_ambient_light : public dv_light {
    	dv_ambient_light() : dv_light() {}
    };
}