#pragma once

#include <cstdint>

namespace devue::plugins {
    class dv_plugin_texture_importer {
    public:
        virtual void init()    = 0;
        virtual void cleanup() = 0;

        virtual uint32_t texture_version() = 0;
        virtual uint32_t plugin_version()  = 0;
    };
}