#pragma once

#include "dv_plugin_importer.hpp"
#include "dv_file_type.hpp"

#include <string>
#include <vector>
#include <filesystem>

#define DV_API extern "C" __declspec(dllexport)

namespace devue::plugins {
    class dv_plugin_adapter : public dv_plugin_importer {
    public:
        dv_plugin_adapter()          = default;
        virtual ~dv_plugin_adapter() = default;

    public:
        serialized get_plugin_info() noexcept final override;
        virtual serialized import(const char* filepath) noexcept override = 0;
        
        virtual void init() = 0;
        void cleanup() noexcept final override;

    protected:
        std::string m_name    = "";
        std::string m_author  = "";
        std::string m_website = "";
        std::string m_version = "";

    protected:
        virtual std::vector<dv_file_type> _get_supported_types() = 0;

    private:
        std::vector<char> m_buffer;
        
        friend class dv_texture_plugin_adapter;
        friend class dv_model_plugin_adapter;
    };
}