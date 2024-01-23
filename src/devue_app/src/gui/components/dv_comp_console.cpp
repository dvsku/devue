#include "gui/components/dv_comp_console.hpp"
#include "gui/components/dv_components.hpp"
#include "utilities/dv_util_log.hpp"

using namespace devue;

dv_comp_console::dv_comp_console(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) 
{
    core::dv_util_log::add_source(&m_text);
}

bool dv_comp_console::render() {
    bool& is_executable = m_systems->command.is_executable(dv_commands::flag_show_console);

    ImGui::Begin("Console##Window", &is_executable, ImGuiWindowFlags_HorizontalScrollbar);
    {
        auto view = m_text.view();
        ImGui::TextUnformatted(view.data(), view.data() + view.length());
    }    
    ImGui::End();

    return DV_COMMAND_REPEAT;
}
