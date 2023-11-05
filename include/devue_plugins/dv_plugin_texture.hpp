#pragma once

#include <vector>

namespace devue::plugins {
    struct dv_plugin_texture {
        int width  = 0;
        int height = 0;
        std::vector<uint8_t> data;
    };
}