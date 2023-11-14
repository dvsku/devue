#pragma once

#include "dv_plugin_adapter.hpp"
#include "dv_plugin_model.hpp"

#define DV_API extern "C" __declspec(dllexport)

namespace devue::plugins {
    class dv_model_plugin_adapter : public dv_plugin_adapter {
    public:
        serialized import(const char* filepath) final override;

    protected:
        virtual void _init() noexcept override = 0;
        virtual std::vector<dv_file_type> _get_supported_types() noexcept override = 0;
        virtual dv_plugin_model _import(const std::filesystem::path& filepath) noexcept = 0;
    };
}