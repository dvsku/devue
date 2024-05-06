#pragma once

#include <cstdint>

namespace devue::core {
    struct dv_scene_texture {
    	uint32_t texture_id = 0U;
        uint32_t width      = 0U;
        uint32_t height     = 0U;
        uint8_t  components = 0U;
    };
}