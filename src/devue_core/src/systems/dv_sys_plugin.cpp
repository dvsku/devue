#include "systems/dv_sys_plugin.hpp"
#include "exceptions/dv_exception.hpp"
#include "utilities/dv_util_log.hpp"

#include <windows.h>
#include <filesystem>

using namespace devue::core;

void release_handle(HMODULE handle);


dv_sys_plugin::~dv_sys_plugin() {
    for (auto& [uuid, plugin] : m_texture_plugins)
        release_plugin(plugin);
}

void dv_sys_plugin::prepare() {
    create_texture_plugins();
}

void dv_sys_plugin::release_plugin(dv_texture_plugin& plugin) {
    if (plugin.m_importer)
        plugin.m_importer->cleanup();

    if (plugin.m_handle) 
        release_handle(plugin.m_handle);

    plugin.m_handle   = nullptr;
    plugin.m_importer = nullptr;

    DV_LOG("Unloaded texture plugin `{}`", plugin.filename);
}

void dv_sys_plugin::create_texture_plugins() {
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

        if (m_texture_plugins.contains(uuid))
            continue;

        try {
            m_texture_plugins[uuid] = std::move(load_texture_plugin(filepath));
        }
        catch (...) {}

        DV_LOG("Loaded texture plugin `{}`", m_texture_plugins[uuid].filename);
    }
}

dv_texture_plugin dv_sys_plugin::load_texture_plugin(const std::filesystem::path& path) {
    HMODULE handle = LoadLibrary(path.string().c_str());
    if (!handle)
        throw DV_EXCEPTION("");

    auto create_fn_addr = GetProcAddress(handle, "create");
    if (!create_fn_addr)
        throw DV_EXCEPTION("");

    typedef devue::plugins::dv_plugin_texture_importer* create_fn();
    create_fn* create = (create_fn*)create_fn_addr;
    if (!create)
        throw DV_EXCEPTION("");

    devue::plugins::dv_plugin_texture_importer* importer = create();
    if (!importer)
        throw DV_EXCEPTION("");

    dv_texture_plugin plugin;
    plugin.m_handle   = handle;
    plugin.m_importer = importer;

    plugin.filename        = path.filename().string();
    plugin.name            = path.filename().replace_extension("").string();
    plugin.plugin_version  = importer->plugin_version();
    plugin.texture_version = importer->texture_version();

    return plugin;
}

void release_handle(HMODULE handle) {
    auto release_fn_addr = GetProcAddress(handle, "release");
    if (!release_fn_addr)
        throw DV_EXCEPTION("");

    typedef void release_fn();
    release_fn* release = (release_fn*)release_fn_addr;
    if (!release)
        throw DV_EXCEPTION("");

    release();

    FreeLibrary(handle);
}