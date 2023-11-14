#include "dv_plugin_adapter.hpp"
#include "dv_serialization.hpp"

using namespace devue::plugins;

dv_plugin_adapter::dv_plugin_adapter() { }

dv_plugin_adapter::~dv_plugin_adapter() {}

dv_plugin_importer::serialized dv_plugin_adapter::get_name() {
    dv_bin_writer bw(m_buffer);
    bw << m_name;

    return { m_buffer.size(), m_buffer.data() };
}

dv_plugin_importer::serialized dv_plugin_adapter::get_author() {
    dv_bin_writer bw(m_buffer);
    bw << m_author;

    return { m_buffer.size(), m_buffer.data() };
}

dv_plugin_importer::serialized dv_plugin_adapter::get_link() {
    dv_bin_writer bw(m_buffer);
    bw << m_link;

    return { m_buffer.size(), m_buffer.data() };
}

uint8_t dv_plugin_adapter::get_plugin_version_major() {
    return m_plugin_version_major;
}

float dv_plugin_adapter::get_plugin_version_minor() {
    return m_plugin_version_minor;
}

uint32_t dv_plugin_adapter::get_internal_version() {
    return 1U;
}

void dv_plugin_adapter::init() {
    _init();
}

void dv_plugin_adapter::cleanup() {
    m_buffer.clear();
}

dv_plugin_importer::serialized dv_plugin_adapter::get_suported_types() {
    std::vector<dv_file_type> supported_types = _get_supported_types();

    dv_bin_writer bw(m_buffer);
    bw << supported_types.size();

    for (dv_file_type& file_type : supported_types)
        bw << file_type.name << file_type.extensions;

    return { m_buffer.size(), m_buffer.data() };
}
