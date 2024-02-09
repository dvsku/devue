#include "systems/dv_sys_plugin.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "exceptions/dv_exception.hpp"
#include "utilities/dv_util_log.hpp"

#include <windows.h>
#include <filesystem>

using namespace devue;
using namespace devue::core;

///////////////////////////////////////////////////////////////////////////////
// INTERNAL FORWARD

struct plugin_handle {
    HMODULE handle                         = nullptr;
    plugins::devue_plugin_interface* iface = nullptr;
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
    if (!std::filesystem::exists("./plugins/") || !std::filesystem::is_directory("./plugins/"))
        return;

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

        if (plugins.contains(uuid))
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
    for (auto& [uuid, plugin] : plugins)
        release_plugin(plugin);

    plugins.clear();
}

void dv_sys_plugin::release_plugin(dv_plugin& plugin) {
    if (plugin.m_iface)
        plugin.m_iface->cleanup();

    if (plugin.m_handle)
        release_handle(plugin.m_handle);

    plugin.m_handle = nullptr;
    plugin.m_iface  = nullptr;

    DV_LOG("Released plugin `{}`", plugin.filename);
}

void dv_sys_plugin::load_plugin(const std::filesystem::path& path, devue::uuid uuid) {
    auto phandle = create_handle(path.string().c_str());

    dv_plugin plugin;
    plugin.m_handle = phandle.handle;
    plugin.m_iface  = phandle.iface;
    plugin.filename = path.filename().string();

    plugin.prepare();

    plugins[uuid] = std::move(plugin);

    DV_LOG("Loaded plugin `{}` from `{}`.", plugins[uuid].name, plugins[uuid].filename);
}

///////////////////////////////////////////////////////////////////////////////
// INTERNAL

plugin_handle create_handle(const char* path) {
    typedef plugins::devue_plugin_interface* fn_create();

    HMODULE handle = LoadLibrary(path);
    if (!handle)
        throw dv_exception("");

    auto       create_fn_addr = GetProcAddress(handle, "create");
    fn_create* create_fn      = (fn_create*)create_fn_addr;

    if (!create_fn_addr || !create_fn)
        throw dv_exception("");

    plugins::devue_plugin_interface* iface = create_fn();
    if (!iface)
        throw dv_exception("");

    return { handle, iface };
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