#pragma once

#include "dv_plugin_texture_importer.hpp"

#include <string>

namespace devue::core {
    class dv_texture_plugin {
    public:
        std::string name = "";
        
        uint32_t texture_version = 0x0001;
        uint32_t plugin_version  = 0x0001;

    private:
        plugins::dv_plugin_texture_importer* m_plugin = nullptr;
    };
}