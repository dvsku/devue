#include "gui/components/dv_comp_console.hpp"
#include "gui/dv_components.hpp"

using namespace devue;

dv_comp_console::dv_comp_console(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) 
{
    libutil::log::create_stream("gui_console");
}

dv_comp_console::~dv_comp_console() {
    libutil::log::remove_stream("gui_console");
}

libgui::command::state dv_comp_console::render() {
    bool* visible = &m_systems->command.is_set_to_execute(dv_commands::flag_show_console);

    if(ImGui::Begin("Console##Window", visible, ImGuiWindowFlags_HorizontalScrollbar)) {
        auto content = libutil::log::get_stream_content("gui_console");
        ImGui::TextUnformatted(content.c_str());
    }    
    ImGui::End();

    return libgui::command::state::repeat;
}
