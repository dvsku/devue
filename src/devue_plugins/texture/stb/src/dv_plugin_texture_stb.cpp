#include "texture/stb/dv_plugin_texture_stb.hpp"

#include <memory>

using namespace devue::plugins;

std::unique_ptr<dv_plugin_texture_stb> g_plugin = nullptr;

void dv_plugin_texture_stb::init() {

}

void dv_plugin_texture_stb::cleanup() {

}

uint32_t dv_plugin_texture_stb::texture_version() {
    return 15;
}

uint32_t dv_plugin_texture_stb::plugin_version() {
    return 20;
}


///////////////////////////////////////////////////////////////////////////////
// C API

DV_API devue::plugins::dv_plugin_texture_importer* create() {
    if (g_plugin) return g_plugin.get();

    g_plugin = std::make_unique<dv_plugin_texture_stb>();
    return g_plugin.get();
}

DV_API void release() {
    return g_plugin.reset();
}
