#include "gui/components/dv_comp_console.hpp"
#include "gui/dv_components.hpp"

using namespace devue;

dv_comp_console::dv_comp_console(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) 
{
    dvsku::util_log::create_source("gui_console", &m_text);
}

dv_comp_console::~dv_comp_console() {
    dvsku::util_log::remove_source("gui_console");
}

libgui::command::state dv_comp_console::render() {
    bool* visible = &m_systems->command.is_set_to_execute(dv_commands::flag_show_console);

    if(ImGui::Begin("Console##Window", visible, ImGuiWindowFlags_HorizontalScrollbar)) {
        auto view = m_text.view();
        ImGui::TextUnformatted(view.data(), view.data() + view.length());
    }    
    ImGui::End();

    return libgui::command::state::repeat;
}
