#include "systems/dv_sys_plugin.hpp"

#include <filesystem>

using namespace devue::core;

void load_texture_plugins(std::unordered_map<devue::uuid, devue::core::dv_texture_plugin>& plugins);
dv_texture_plugin load_texture_plugin(const std::filesystem::path& path);

void dv_sys_plugin::prepare() {
    load_texture_plugins(m_texture_plugins);
}

void load_texture_plugins(std::unordered_map<devue::uuid, devue::core::dv_texture_plugin>& plugins) {
    for (const auto& entry : std::filesystem::directory_iterator("./")) {
        // Make sure it's a file
        if (!std::filesystem::is_regular_file(entry)) 
            continue;
        
        std::filesystem::path filepath = entry.path();

        // Make sure it's a dll
        if (filepath.extension() != ".dll")
            continue;

        // Make sure it's a texture plugin
        if (!filepath.filename().string().starts_with("dv_plg_tex"))
            continue;

        devue::uuid uuid = dv_util_uuid::create(filepath.string());

        if (plugins.contains(uuid))
            continue;

        try {
            plugins[uuid] = load_texture_plugin(filepath);
        }
        catch (...) {}
    }
}

dv_texture_plugin load_texture_plugin(const std::filesystem::path& path) {
    dv_texture_plugin plugin;
    plugin.name = path.filename().replace_extension("").string();

    return plugin;
}
