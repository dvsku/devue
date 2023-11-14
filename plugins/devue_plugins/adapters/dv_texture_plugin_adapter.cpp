#include "dv_texture_plugin_adapter.hpp"
#include "dv_serialization.hpp"

using namespace devue::plugins;

dv_plugin_importer::serialized dv_texture_plugin_adapter::import(const char* filepath) {
    dv_plugin_texture texture = _import(filepath);

    dv_bin_writer bw(m_buffer);
    bw << texture.width << texture.height << texture.data;

    return { m_buffer.size(), m_buffer.data() };
}
