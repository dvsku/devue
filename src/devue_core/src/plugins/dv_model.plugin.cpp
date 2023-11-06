#include "plugins/dv_model_plugin.hpp"
#include "dv_serialization.hpp"

using namespace devue::core;
using namespace devue::plugins;

dv_plugin_model dv_model_plugin::import(const std::string & filepath) {
    dv_plugin_model model;
    dv_plugin_importer::serialized serialized;

    serialized = m_importer->import(filepath.c_str());

    if (!serialized)
        throw;

    m_importer->cleanup();

    return model;
}