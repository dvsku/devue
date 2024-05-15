#pragma once

#include "glm/vec3.hpp"

namespace devue::core {
    struct dv_bounding_box {
        glm::vec3 minimum;
        glm::vec3 maximum;
    };
}