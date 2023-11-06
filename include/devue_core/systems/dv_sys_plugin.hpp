#pragma once

#include "plugins/dv_texture_plugin.hpp"
#include "plugins/dv_model_plugin.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>
#include <filesystem>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_plugin {
    public:
        dv_sys_plugin() = delete;
        dv_sys_plugin(dv_systems_bundle* systems);
        ~dv_sys_plugin();

        dv_sys_plugin(const dv_sys_plugin&) = delete;
        dv_sys_plugin(dv_sys_plugin&&) = delete;

        dv_sys_plugin& operator=(const dv_sys_plugin&) = delete;
        dv_sys_plugin& operator=(dv_sys_plugin&&) = delete;

        void prepare();
        void release_plugin(dv_plugin& plugin);

    private:
        dv_systems_bundle* m_systems;
        std::unordered_map<devue::uuid, devue::core::dv_texture_plugin> m_texture_plugins;
        std::unordered_map<devue::uuid, devue::core::dv_model_plugin>   m_model_plugins;

    private:
        void create_texture_plugins();
        void create_model_plugins();

        dv_texture_plugin load_texture_plugin(const std::filesystem::path& path);
        dv_model_plugin load_model_plugin(const std::filesystem::path& path);
    };
}