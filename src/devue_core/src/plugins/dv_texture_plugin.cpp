#include "plugins/dv_texture_plugin.hpp"
#include "dv_serialization.hpp"

using namespace devue::core;
using namespace devue::plugins;

void dv_texture_plugin::prepare() {
    m_importer->init();

    dv_bin_reader br;
    dv_plugin_texture_importer::serialized serialized;

    serialized = m_importer->get_name();
    if (serialized.size && serialized.data) {
        br.set(serialized.data, serialized.size);
        br >> name;
    }

    serialized = m_importer->get_author();
    if (serialized.size && serialized.data) {
        br.set(serialized.data, serialized.size);
        br >> author;
    }

    serialized = m_importer->get_link();
    if (serialized.size && serialized.data) {
        br.set(serialized.data, serialized.size);
        br >> link;
    }
       
    plugin_version_major    = m_importer->get_plugin_version_major();
    plugin_version_minor    = m_importer->get_plugin_version_minor();
    plugin_version_internal = m_importer->get_internal_version();
}