#pragma once

#include "dv_plugin_importer.hpp"
#include "dv_file_type.hpp"

#include <string>
#include <vector>
#include <filesystem>

#define DV_API extern "C" __declspec(dllexport)

namespace devue::plugins {
    class sbuffer;

    class dv_plugin_adapter : public dv_plugin_importer {
    public:
        dv_plugin_adapter();
        virtual ~dv_plugin_adapter();

        ///////////////////////////////////////////////////////////////////////
        // PLUGIN DETAILS

        serialized get_name()   final override;
        serialized get_author() final override;
        serialized get_link()   final override;

        uint8_t get_plugin_version_major() final override;
        float get_plugin_version_minor()   final override;
        uint32_t get_internal_version()    final override;

        ///////////////////////////////////////////////////////////////////////
        // PLUGIN IMPL

        void init()    final override;
        void cleanup() final override;

        serialized get_suported_types() final override;
        virtual serialized import(const char* filepath) = 0;

    protected:
        std::string m_name   = "";
        std::string m_author = "";
        std::string m_link   = "";

        uint8_t m_plugin_version_major = 1U;
        float m_plugin_version_minor   = 0.0f;

    protected:
        virtual void _init() noexcept = 0;
        virtual  std::vector<dv_file_type> _get_supported_types() noexcept = 0;

    private:
        std::vector<uint8_t> m_buffer{};
        friend class dv_texture_plugin_adapter;
        friend class dv_model_plugin_adapter;
    };
}