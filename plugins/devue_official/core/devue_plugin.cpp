#include "devue_plugin.hpp"

using namespace devue::plugins;

void devue_plugin::impl_init() {
    impl.impl_init(*this);
}

std::vector<file_type> devue_plugin::impl_get_model_types() {
    return impl.impl_get_model_types();
}

std::vector<file_type> devue_plugin::impl_get_texture_types() {
    return impl.impl_get_texture_types();
}

devue_plugin_model devue_plugin::impl_import_model(const std::filesystem::path& filepath) {
    return impl.impl_import_model(filepath);
}

devue_plugin_texture devue_plugin::impl_import_texture(const std::filesystem::path& filepath) {
    return impl.impl_import_texture(filepath);
}

///////////////////////////////////////////////////////////////////////////////
// C API - DO NOT CHANGE

#include <memory>

std::unique_ptr<devue_plugin> plugin_instance = nullptr;

DV_API devue_plugin_interface* create() {
    if (plugin_instance) return plugin_instance.get();

    try {
        plugin_instance = std::make_unique<devue_plugin>();
        plugin_instance->impl_init();
    }
    catch (...) {
        return nullptr;
    }

    return plugin_instance.get();
}

DV_API void release() {
    return plugin_instance.reset();
}