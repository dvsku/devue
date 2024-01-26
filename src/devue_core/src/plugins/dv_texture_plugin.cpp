#include "plugins/dv_texture_plugin.hpp"
#include "json/include/json.hpp"

using namespace devue::core;
using namespace devue::plugins;

dv_plugin_texture dv_texture_plugin::import(const std::string& filepath) {
    dv_plugin_texture texture;
    dv_plugin_importer::serialized serialized;

    serialized = m_importer->import(filepath.c_str());

    if (!serialized)
        throw;

    nlohmann::json json = nlohmann::json::from_cbor(serialized.data, serialized.data + serialized.size);

    if (json.contains("width") && json["width"].is_number())
        texture.width = json["width"];

    if (json.contains("height") && json["height"].is_number())
        texture.height = json["height"];

    if (json.contains("data") && json["data"].is_binary())
        texture.data = std::move(json["data"].get_binary());

    return texture;
}

void dv_texture_plugin::cleanup() {
    m_importer->cleanup();
}