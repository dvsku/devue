#include "plugins/dv_plugin.hpp"
#include "dv_serialization.hpp"

using namespace devue::core;
using namespace devue::plugins;

void dv_plugin::prepare() {
    m_importer->init();

    dv_bin_reader br;
    dv_plugin_importer::serialized serialized;

    serialized = m_importer->get_name();
    if (serialized) {
        br.set(serialized.data, serialized.size);
        br >> name;
    }

    serialized = m_importer->get_author();
    if (serialized) {
        br.set(serialized.data, serialized.size);
        br >> author;
    }

    serialized = m_importer->get_link();
    if (serialized) {
        br.set(serialized.data, serialized.size);
        br >> link;
    }

    plugin_version_major    = m_importer->get_plugin_version_major();
    plugin_version_minor    = m_importer->get_plugin_version_minor();
    plugin_version_internal = m_importer->get_internal_version();

    serialized = m_importer->get_suported_types();
    if (serialized) {
        br.set(serialized.data, serialized.size);

        size_t size = 0U;
        br >> size;

        for (size_t i = 0; i < size; i++) {
            dv_file_type file_type;
            br >> file_type.name >> file_type.extensions;
            supported_file_types.push_back(file_type);
        }
    }
}