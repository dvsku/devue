#pragma once

#include <vector>

namespace devue::plugins {
    struct devue_plugin_texture {
        int                  width  = 0;
        int                  height = 0;
        std::vector<uint8_t> data;
    };
}