#include "systems/dv_sys_plugin.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "exceptions/dv_exception.hpp"
#include "utilities/dv_util_log.hpp"

#include <windows.h>
#include <filesystem>

using namespace devue::core;

///////////////////////////////////////////////////////////////////////////////
// INTERNAL FORWARD

struct plugin_handle {
    HMODULE handle                               = nullptr;
    devue::plugins::dv_plugin_importer* importer = nullptr;
};

static plugin_handle create_handle(const char* path);
static void release_handle(HMODULE handle);

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_sys_plugin::dv_sys_plugin(dv_systems_bundle* systems) 
    : m_systems(systems) {}

dv_sys_plugin::~dv_sys_plugin() {
    m_systems->texture.release_importers();

    for (auto& [uuid, plugin] : m_texture_plugins)
        release_plugin(plugin);
}

void dv_sys_plugin::prepare() {
    create_model_plugins();
    create_texture_plugins();

    for (auto& [uuid, plugin] : m_model_plugins) {
        m_systems->model.create_importer(
            {
                plugin.supported_file_types,
                [&](const std::string& filepath) {
                    return plugin.import(filepath);
                }
            }
        );
    }

    m_systems->model.update_supported_file_types();

    for (auto& [uuid, plugin] : m_texture_plugins) {
        m_systems->texture.create_importer(
            {
                plugin.supported_file_types,
                [&](const std::string& filepath) {
                    return plugin.import(filepath);
                }
            }
        );
    }
}

void dv_sys_plugin::release_plugin(dv_plugin& plugin) {
    if (plugin.m_importer)
        plugin.m_importer->cleanup();

    if (plugin.m_handle) 
        release_handle(plugin.m_handle);

    plugin.m_handle   = nullptr;
    plugin.m_importer = nullptr;

    DV_LOG("Released plugin `{}`", plugin.filename);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

void dv_sys_plugin::create_texture_plugins() {
    for (const auto& entry : std::filesystem::directory_iterator("./plugins/")) {
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
        catch (...) {
            DV_LOG("Failed to load `{}` texture plugin.", filepath.filename().string());
            continue;
        }

        DV_LOG("Loaded texture plugin `{}` from `{}`.", m_texture_plugins[uuid].name, m_texture_plugins[uuid].filename);
    }
}

void dv_sys_plugin::create_model_plugins() {
    for (const auto& entry : std::filesystem::directory_iterator("./plugins/")) {
        // Make sure it's a file
        if (!std::filesystem::is_regular_file(entry))
            continue;

        std::filesystem::path filepath = entry.path();

        // Make sure it's a dll
        if (filepath.extension() != ".dll")
            continue;

        // Make sure it's a texture plugin
        if (!filepath.filename().string().starts_with("dv_plg_mod"))
            continue;

        devue::uuid uuid = dv_util_uuid::create(filepath.string());

        if (m_model_plugins.contains(uuid))
            continue;

        try {
            m_model_plugins[uuid] = std::move(load_model_plugin(filepath));
        }
        catch (...) {
            DV_LOG("Failed to load `{}` model plugin.", filepath.filename().string());
            continue;
        }

        DV_LOG("Loaded model plugin `{}` from `{}`.", m_model_plugins[uuid].name, m_model_plugins[uuid].filename);
    }
}

dv_texture_plugin dv_sys_plugin::load_texture_plugin(const std::filesystem::path& path) {
    auto phandle = create_handle(path.string().c_str());

    dv_texture_plugin plugin;
    plugin.m_handle   = phandle.handle;
    plugin.m_importer = phandle.importer;
    plugin.filename   = path.filename().string();

    plugin.prepare();
    return plugin;
}

dv_model_plugin dv_sys_plugin::load_model_plugin(const std::filesystem::path& path) {
    auto phandle = create_handle(path.string().c_str());

    dv_model_plugin plugin;
    plugin.m_handle   = phandle.handle;
    plugin.m_importer = phandle.importer;
    plugin.filename   = path.filename().string();

    plugin.prepare();
    return plugin;
}

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

plugin_handle create_handle(const char* path) {
    HMODULE handle = LoadLibrary(path);
    if (!handle)
        throw dv_exception("");

    auto create_fn_addr = GetProcAddress(handle, "create");
    if (!create_fn_addr)
        throw dv_exception("");

    typedef devue::plugins::dv_plugin_importer* create_fn();
    create_fn* create = (create_fn*)create_fn_addr;
    if (!create)
        throw dv_exception("");

    devue::plugins::dv_plugin_importer* importer = create();
    if (!importer)
        throw dv_exception("");

    return { handle, importer };
}

void release_handle(HMODULE handle) {
    auto release_fn_addr = GetProcAddress(handle, "release");
    if (!release_fn_addr)
        throw dv_exception("");

    typedef void release_fn();
    release_fn* release = (release_fn*)release_fn_addr;
    if (!release)
        throw dv_exception("");

    release();

    FreeLibrary(handle);
}