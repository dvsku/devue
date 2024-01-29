#pragma once

#include "devue_plugin_model.hpp"
#include "devue_plugin_texture.hpp"

#include <filesystem>

namespace devue::plugins {
    struct file_type;
    class  devue_plugin;

    class devue_plugin_impl {
    public:
        void                   impl_init(devue_plugin& plugin);
        std::vector<file_type> impl_get_model_types();
        std::vector<file_type> impl_get_texture_types();
        devue_plugin_model     impl_import_model(const std::filesystem::path& filepath);
        devue_plugin_texture   impl_import_texture(const std::filesystem::path& filepath);
    };
}