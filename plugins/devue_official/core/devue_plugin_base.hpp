#pragma once

#include "devue_plugin_interface.hpp"
#include "devue_plugin_model.hpp"
#include "devue_plugin_texture.hpp"

#include <string>
#include <vector>
#include <filesystem>

#define DV_API extern "C" __declspec(dllexport)

namespace devue::plugins {
    struct file_type {
        std::string name       = "";
        std::string extensions = "";
    };

    class devue_plugin_base : public devue_plugin_interface {
    public:
        std::string name    = "";
        std::string author  = "";
        std::string website = "";
        std::string version = "";

    public:
        devue_plugin_base()          = default;
        virtual ~devue_plugin_base() = default;

    protected:
        virtual void                   impl_init()                                                = 0;
        virtual std::vector<file_type> impl_get_model_types()                                     = 0;
        virtual std::vector<file_type> impl_get_texture_types()                                   = 0;
        virtual devue_plugin_model     impl_import_model(const std::filesystem::path& filepath)   = 0;
        virtual devue_plugin_texture   impl_import_texture(const std::filesystem::path& filepath) = 0;

    private:
        std::vector<char> m_buffer;
        std::string       m_error_msg = "";

    private:
        serialized get_plugin_info() noexcept final override;
        serialized get_error_message() noexcept final override;
        serialized import_model(const char* filepath) noexcept final override;
        serialized import_texture(const char* filepath) noexcept final override;
        void cleanup() noexcept final override;
    };
}