#include "plugins/dv_texture_plugin.hpp"
#include "dv_serialization.hpp"

using namespace devue::core;
using namespace devue::plugins;

dv_plugin_texture dv_texture_plugin::import(const std::string& filepath) {
    dv_plugin_texture texture;
    dv_plugin_importer::serialized serialized;

    serialized = m_importer->import(filepath.c_str());

    if (!serialized.size || !serialized.data)
        throw;

    dv_bin_reader br(serialized.data, serialized.size);
    br >> texture.width >> texture.height >> texture.data;

    m_importer->cleanup();

    return texture;
}
