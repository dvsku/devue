#pragma once

#include "plugins/dv_plugin.hpp"
#include "dv_plugin_texture.hpp"

#include <windows.h>
#include <string>

namespace devue::core {
    class dv_texture_plugin : public dv_plugin {
    public:
        dv_texture_plugin()                         = default;
        dv_texture_plugin(const dv_texture_plugin&) = delete;
        dv_texture_plugin(dv_texture_plugin&&)      = default;

        dv_texture_plugin& operator=(const dv_texture_plugin&) = delete;
        dv_texture_plugin& operator=(dv_texture_plugin&&)      = default;

    public:
        devue::plugins::dv_plugin_texture import(const std::string& filepath);
        void cleanup();
    };
}