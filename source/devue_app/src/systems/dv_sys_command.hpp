#pragma once

#include <dv_gui_opengl/dv_gui_opengl.hpp>
#include <map>

namespace devue {
    struct dv_systems;
    struct dv_components;

    enum class dv_commands : uint16_t {
        flag_show_console,
        flag_show_texture,
        flag_show_modal_import,
        flag_show_modal_plugins,
        flag_show_modal_about
    };

    class dv_sys_command : public dvsku::dv_sys_command<dv_commands> {
    public:
        dv_sys_command() = delete;
        dv_sys_command(dv_systems* systems);

        dv_sys_command(const dv_sys_command&) = delete;
        dv_sys_command(dv_sys_command&&)      = delete;

        dv_sys_command& operator=(const dv_sys_command&) = delete;
        dv_sys_command& operator=(dv_sys_command&&)      = delete;

    public:
        // Prepare GUI commands
        bool prepare(dv_components* components);

    private:
        dv_systems* m_systems = nullptr;
    };
}