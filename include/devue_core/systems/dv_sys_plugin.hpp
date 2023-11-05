#pragma once

#include "plugins/dv_texture_plugin.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>
#include <filesystem>

namespace devue::core {
    class dv_sys_plugin {
    public:
        ~dv_sys_plugin();

        void prepare();
        void release_plugin(dv_texture_plugin& plugin);

    private:
        std::unordered_map<devue::uuid, devue::core::dv_texture_plugin> m_texture_plugins;

    private:
        void create_texture_plugins();
        dv_texture_plugin load_texture_plugin(const std::filesystem::path& path);
    };
}