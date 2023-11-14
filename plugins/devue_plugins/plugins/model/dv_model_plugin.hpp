#pragma once

#include "dv_model_plugin_adapter.hpp"
#include "assimp/Importer.hpp"

#include <string>

namespace devue::plugins {
    class dv_model_plugin : public dv_model_plugin_adapter {
    protected:
        virtual void _init() noexcept final override;
        virtual std::vector<dv_file_type> _get_supported_types() noexcept final override;
        virtual dv_plugin_model _import(const std::filesystem::path& filepath) noexcept final override;

    private:
        Assimp::Importer importer;
    };
}

DV_API devue::plugins::dv_plugin_importer* create();
DV_API void release();