#pragma once

#include "devue_plugin_base.hpp"
#include "assimp/Importer.hpp"

namespace devue::plugins {
    class dv_model_plugin : public devue_plugin_base {
    public:
        virtual void impl_init() final override;

    protected:   
        std::vector<file_type> impl_get_model_types()                                     final override;
        std::vector<file_type> impl_get_texture_types()                                   final override;
        devue_plugin_model     impl_import_model(const std::filesystem::path& filepath)   final override;
        devue_plugin_texture   impl_import_texture(const std::filesystem::path& filepath) final override;

    private:
        Assimp::Importer importer;
    };
}

DV_API devue::plugins::devue_plugin_interface* create();
DV_API void                                    release();