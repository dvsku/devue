#pragma once

#include <string>
#include <vector>

namespace devue::plugins {
    class devue_plugin_mesh {
    public:
        std::string           name           = "";
        uint64_t              material_index = 0U;
        std::vector<uint32_t> indices;
    };
}