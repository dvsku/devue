#pragma once

#include "glm/glm.hpp"

namespace devue::core {
    struct dv_util_mouse {
    	inline static glm::dvec2 position{};
    	inline static glm::dvec2 delta{};
    };
}