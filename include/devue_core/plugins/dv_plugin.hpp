#pragma once

#include <windows.h>
#include <string>

namespace devue::core {
    class dv_plugin {
    public:     
        std::string name     = "";
        std::string filename = "";

        uint32_t plugin_version = 0x0001;

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