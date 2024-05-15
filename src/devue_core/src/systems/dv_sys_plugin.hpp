#pragma once

#include "plugins/dv_plugin.hpp"

#include <dv_utilities.hpp>
#include <unordered_map>
#include <filesystem>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_plugin {
    public:
        std::unordered_map<dvsku::uuid, dv_plugin> plugins;

    public:
        dv_sys_plugin() = delete;
        dv_sys_plugin(dv_systems_bundle* systems);
        ~dv_sys_plugin();

        dv_sys_plugin(const dv_sys_plugin&) = delete;
        dv_sys_plugin(dv_sys_plugin&&) = delete;

        dv_sys_plugin& operator=(const dv_sys_plugin&) = delete;
        dv_sys_plugin& operator=(dv_sys_plugin&&) = delete;

        void prepare();
        void release();

        void reload_plugins();

        std::vector<std::string> get_model_file_types();
        std::vector<std::string> get_texture_file_types();

        bool is_supported_model_type(const std::string& path);

    private:
        dv_systems_bundle* m_systems;

    private:
        void prepare_plugins();
        void release_plugins();

        void release_plugin(dv_plugin& plugin);

        void load_plugin(const std::filesystem::path& path, dvsku::uuid uuid);
    };
}