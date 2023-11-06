#pragma once

#include "dv_file_type.hpp"

#include <windows.h>
#include <string>
#include <vector>

namespace devue::core {
    class dv_plugin {
    public:
        std::string filename = "";
        std::string name     = "";
        std::string author   = "";
        std::string link     = "";

        uint8_t  plugin_version_major    = 0U;
        float    plugin_version_minor    = 0.0f;
        uint32_t plugin_version_internal = 0U;

        std::vector<plugins::dv_file_type> supported_file_types;

        friend class dv_sys_plugin;

    public:
        dv_plugin()                 = default;
        dv_plugin(const dv_plugin&) = delete;
        dv_plugin(dv_plugin&&)      = default;

        virtual ~dv_plugin() = default;

        dv_plugin& operator=(const dv_plugin&)  = delete;
        dv_plugin& operator=(dv_plugin&&)       = default;

    protected:
        HMODULE m_handle = nullptr;
    };
}