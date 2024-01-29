#pragma once

#include "devue_plugin_material.hpp"

#include <vector>

namespace devue::plugins {
    class devue_plugin_mesh {
    public:
        std::string           name = "";
        devue_plugin_material material{};
        std::vector<uint32_t> indices;
    };
}