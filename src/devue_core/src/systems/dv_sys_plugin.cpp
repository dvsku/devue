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
    HMODULE handle                                       = nullptr;
    devue::plugins::dv_plugin_importer* importer         = nullptr;
    devue::plugins::dv_plugin_importer::plugin_type type = devue::plugins::dv_plugin_importer::plugin_type::undefined;
};

static plugin_handle create_handle(const char* path);
static void release_handle(HMODULE handle);

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_sys_plugin::dv_sys_plugin(dv_systems_bundle* systems) 
    : m_systems(systems) {}

dv_sys_plugin::~dv_sys_plugin() {
    release();
}

void dv_sys_plugin::prepare() {
    prepare_plugins();
    m_systems->model.update_supported_file_types();
}

void dv_sys_plugin::release() {
    release_plugins();
    m_systems->model.update_supported_file_types();
}

void dv_sys_plugin::reload_plugins() {
    release_plugins();
    prepare_plugins();
    m_systems->model.update_supported_file_types();
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

void dv_sys_plugin::prepare_plugins() {
    for (const auto& entry : std::filesystem::directory_iterator("./plugins/")) {
        // Make sure it's a file
        if (!std::filesystem::is_regular_file(entry))
            continue;

        std::filesystem::path filepath = entry.path();

        // Make sure it's a dll
        if (filepath.extension() != ".dll")
            continue;

        // Make sure it's a texture plugin
        if (!filepath.filename().string().starts_with("dv_plg_"))
            continue;

        devue::uuid uuid = dv_util_uuid::create(filepath.string());

        if (model_plugins.contains(uuid) || texture_plugins.contains(uuid))
            continue;

        try {
            load_plugin(filepath, uuid);
        }
        catch (const std::exception& e) {
            DV_LOG("Failed to load `{}` plugin. | {}", filepath.filename().string(), e.what());
            continue;
        }
        catch (...) {
            DV_LOG("Failed to load `{}` plugin.", filepath.filename().string());
            continue;
        }
    }
}

void dv_sys_plugin::release_plugins() {
    for (auto& [uuid, plugin] : model_plugins)
        release_plugin(plugin);

    for (auto& [uuid, plugin] : texture_plugins)
        release_plugin(plugin);
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

void dv_sys_plugin::load_plugin(const std::filesystem::path& path, devue::uuid uuid) {
    auto phandle = create_handle(path.string().c_str());

    if (phandle.type == plugins::dv_plugin_importer::plugin_type::undefined)
        throw dv_exception("Failed to determine plugin type.");

    if (phandle.type == plugins::dv_plugin_importer::plugin_type::model) {
        dv_model_plugin plugin;
        plugin.m_handle   = phandle.handle;
        plugin.m_importer = phandle.importer;
        plugin.filename   = path.filename().string();

        plugin.prepare();

        model_plugins[uuid] = std::move(plugin);

        DV_LOG("Loaded texture plugin `{}` from `{}`.", model_plugins[uuid].name, model_plugins[uuid].filename);
    }
    else if (phandle.type == plugins::dv_plugin_importer::plugin_type::texture) {
        dv_texture_plugin plugin;
        plugin.m_handle   = phandle.handle;
        plugin.m_importer = phandle.importer;
        plugin.filename   = path.filename().string();

        plugin.prepare();

        texture_plugins[uuid] = std::move(plugin);

        DV_LOG("Loaded texture plugin `{}` from `{}`.", texture_plugins[uuid].name, texture_plugins[uuid].filename);
    }
}

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

plugin_handle create_handle(const char* path) {
    typedef devue::plugins::dv_plugin_importer* fn_create();
    typedef uint8_t                             fn_plugin_type();

    HMODULE handle = LoadLibrary(path);
    if (!handle)
        throw dv_exception("");

    auto       create_fn_addr = GetProcAddress(handle, "create");
    fn_create* create_fn      = (fn_create*)create_fn_addr;

    if (!create_fn_addr || !create_fn)
        throw dv_exception("");

    auto            plugin_type_fn_addr = GetProcAddress(handle, "plugin_type");
    fn_plugin_type* plugin_type_fn      = (fn_plugin_type*)plugin_type_fn_addr;

    if (!plugin_type_fn_addr || !plugin_type_fn)
        throw dv_exception("");

    devue::plugins::dv_plugin_importer* importer = create_fn();
    if (!importer)
        throw dv_exception("");

    auto plugin_type = (devue::plugins::dv_plugin_importer::plugin_type)plugin_type_fn();

    return { handle, importer, plugin_type };
}

void release_handle(HMODULE handle) {
    typedef void fn_release();

    auto        release_fn_addr = GetProcAddress(handle, "release");
    fn_release* release_fn      = (fn_release*)release_fn_addr;

    if (!release_fn_addr || !release_fn)
        throw dv_exception("");

    release_fn();
    FreeLibrary(handle);
}