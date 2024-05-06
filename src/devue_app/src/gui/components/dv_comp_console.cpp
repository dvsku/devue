#include "gui/components/dv_comp_console.hpp"
#include "gui/dv_components.hpp"

using namespace devue;

dv_comp_console::dv_comp_console(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) 
{
    dvsku::dv_util_log::create_source("gui_console", &m_text);
}

dv_comp_console::~dv_comp_console() {
    dvsku::dv_util_log::remove_source("gui_console");
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
