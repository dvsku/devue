#pragma once

#include "plugins/dv_plugin.hpp"
#include "dv_plugin_model.hpp"

#include <windows.h>
#include <string>

namespace devue::core {
    class dv_model_plugin : public dv_plugin {
    public:
        dv_model_plugin()                       = default;
        dv_model_plugin(const dv_model_plugin&) = delete;
        dv_model_plugin(dv_model_plugin&&)      = default;

        dv_model_plugin& operator=(const dv_model_plugin&) = delete;
        dv_model_plugin& operator=(dv_model_plugin&&)      = default;

    public:
        devue::plugins::dv_plugin_model import(const std::string & filepath);
        void cleanup();
    };
}