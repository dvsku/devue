#pragma once

#include "misc/dv_file_type.hpp"
#include "devue_plugin_interface.hpp"
#include "devue_plugin_model.hpp"
#include "devue_plugin_texture.hpp"

#include <windows.h>
#include <string>
#include <vector>

namespace devue::core {
    class dv_plugin {
    public:
        std::string filename = "";
        std::string name     = "";
        std::string author   = "";
        std::string website  = "";
        std::string version  = "";

        std::vector<dv_file_type> supported_model_types;
        std::vector<dv_file_type> supported_texture_types;

    public:
        dv_plugin()                 = default;
        dv_plugin(const dv_plugin&) = delete;
        dv_plugin(dv_plugin&&)      = default;

        virtual ~dv_plugin() = default;

        dv_plugin& operator=(const dv_plugin&) = delete;
        dv_plugin& operator=(dv_plugin&&)      = default;

    public:
        virtual void prepare();

        devue::plugins::devue_plugin_model   import_model(const std::string& filepath);
        devue::plugins::devue_plugin_texture import_texture(const std::string& filepath);

        void cleanup();

    protected:
        HMODULE m_handle                                = nullptr;
        devue::plugins::devue_plugin_interface* m_iface = nullptr;

        friend class dv_sys_plugin;
    };
}