#include "systems/dv_sys_command.hpp"
#include "systems/dv_systems.hpp"
#include "gui/dv_components.hpp"
#include "exceptions/dv_exception.hpp"

using namespace devue;

dv_sys_command::dv_sys_command(dv_systems* systems) 
    : m_systems(systems) {}

bool dv_sys_command::prepare(dv_components* components) {
    if (!m_systems) return false;

    commands.emplace(dv_commands::flag_show_console, dv_util_command([components] {
        if (!components) return false;
        return components->console.render();
    }));

    return true;
}

bool& dv_sys_command::is_executable(dv_commands command) {
    if (!commands.contains(command))
        throw core::dv_exception("Command not found.");
    
    return commands[command].executable;
}

void dv_sys_command::set_execute(dv_commands command) {
    if (!commands.contains(command))
        throw core::dv_exception("Command not found.");

    commands[command].executable = true;
}

void dv_sys_command::execute() {
    for (auto& [type, command] : commands)
        command.execute();
}
