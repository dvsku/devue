#pragma once

#include "plugins/dv_plugin.hpp"
#include "dv_plugin_texture_importer.hpp"

#include <windows.h>
#include <string>

namespace devue::core {
    class dv_texture_plugin : private dv_plugin {
    public:
        uint32_t texture_version = 0x0001;

        friend class dv_sys_plugin;

    public:
        dv_texture_plugin()                         = default;
        dv_texture_plugin(const dv_texture_plugin&) = delete;
        dv_texture_plugin(dv_texture_plugin&&)      = default;

        dv_texture_plugin& operator=(const dv_texture_plugin&) = delete;
        dv_texture_plugin& operator=(dv_texture_plugin&&)      = default;

    private:
        plugins::dv_plugin_texture_importer* m_importer = nullptr;
    };
}