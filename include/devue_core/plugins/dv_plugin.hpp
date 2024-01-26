#pragma once

#include "dv_file_type.hpp"
#include "dv_plugin_importer.hpp"

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

        plugins::dv_plugin_importer::plugin_type type = plugins::dv_plugin_importer::plugin_type::undefined;

        std::vector<plugins::dv_file_type> supported_file_types;

    public:
        dv_plugin()                 = default;
        dv_plugin(const dv_plugin&) = delete;
        dv_plugin(dv_plugin&&)      = default;

        virtual ~dv_plugin() = default;

        dv_plugin& operator=(const dv_plugin&)  = delete;
        dv_plugin& operator=(dv_plugin&&)       = default;

        virtual void prepare();

    protected:
        HMODULE m_handle = nullptr;
        plugins::dv_plugin_importer* m_importer = nullptr;

        friend class dv_sys_plugin;
    };
}