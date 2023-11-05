#pragma once

#include "dv_plugin_texture_importer.hpp"

#define DV_API extern "C" __declspec(dllexport)

namespace devue::plugins {
    class dv_plugin_texture_stb : public dv_plugin_texture_importer {
    public:
        void init()    final override;
        void cleanup() final override;

        uint32_t texture_version() final override;
        uint32_t plugin_version()  final override;
    };
}

DV_API devue::plugins::dv_plugin_texture_importer* create();
DV_API void release();