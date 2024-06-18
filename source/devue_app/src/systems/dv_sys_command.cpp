#include "systems/dv_sys_command.hpp"
#include "systems/dv_systems.hpp"
#include "gui/dv_components.hpp"
#include "exceptions/dv_exception.hpp"

using namespace devue;

dv_sys_command::dv_sys_command(dv_systems* systems) 
    : m_systems(systems) {}

bool dv_sys_command::prepare(dv_components* components) {
    if (!m_systems || !components) return false;

    set_command(dv_commands::flag_show_console, libgui::command([components] {
        return components->console.render();
    }));

    set_command(dv_commands::flag_show_texture, libgui::command([components] {
        return components->texture.render();
    }));

    set_command(dv_commands::flag_show_modal_import, libgui::command([components] {
        return components->modal_import.render();
    }));

    set_command(dv_commands::flag_show_modal_plugins, libgui::command([components] {
        return components->modal_plugins.render();
    }));

    set_command(dv_commands::flag_show_modal_about, libgui::command([components] {
        return components->modal_about.render();
    }));

    return true;
}
