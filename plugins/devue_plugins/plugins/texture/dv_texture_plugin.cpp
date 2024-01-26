#include "dv_texture_plugin.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <memory>

using namespace devue::plugins;

// Plugin instance
std::unique_ptr<dv_texture_plugin> g_plugin = nullptr;

///////////////////////////////////////////////////////////////////////////////
// C API

DV_API dv_plugin_importer* create() {
    if (g_plugin) return g_plugin.get();

    try {
        g_plugin = std::make_unique<dv_texture_plugin>();
        g_plugin->init();
    }
    catch (...) {
        return nullptr;
    }

    return g_plugin.get();
}

DV_API void release() {
    return g_plugin.reset();
}

///////////////////////////////////////////////////////////////////////////////
// IMPL

void dv_texture_plugin::init() {
    this->m_name    = "devue texture plugin";
    this->m_author  = "";
    this->m_website = "";
    this->m_version = "1.0.0";
}

std::vector<dv_file_type> dv_texture_plugin::_get_supported_types() {
    return {
        { "JPEG",   ".jpg;.jpeg"},
        { "PNG",    ".png"},
        { "Bitmap", ".bmp"},
    };
}

dv_plugin_texture dv_texture_plugin::_import(const std::filesystem::path& filepath) {
    dv_plugin_texture texture;

    stbi_set_flip_vertically_on_load(true);

    int channels = 0;
    unsigned char* texture_data = 
        stbi_load(filepath.string().c_str(), &texture.width, &texture.height, &channels, 0);

    if (!texture_data) {
        stbi_image_free(texture_data);
        return dv_plugin_texture();
    }
        
    size_t size = (size_t)texture.width * texture.height * channels;
    texture.data.insert(texture.data.begin(), texture_data, texture_data + size);
 
    stbi_image_free(texture_data);

    return texture;
}

