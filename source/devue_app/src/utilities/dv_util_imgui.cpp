#include "utilities/dv_util_imgui.hpp"
#include "dv_theme.hpp"

#include <libgui/theme.hpp>

using namespace devue;
using namespace libgui;

bool dv_util_imgui::collapsible(const char* label, ImGuiTreeNodeFlags flags) {
    theme::push_col(ImGuiCol_Header,        theme::get_col(extensions_col::collapsible));
    theme::push_col(ImGuiCol_HeaderHovered, theme::get_col(extensions_col::collapsible_hovered));
    theme::push_col(ImGuiCol_HeaderActive,  theme::get_col(extensions_col::collapsible_activated));

    bool retval = ImGui::CollapsingHeader(label, flags);

    theme::pop_col(3);
    return retval;
}

bool dv_util_imgui::selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size) {
    theme::push_col(ImGuiCol_Header,        theme::get_col(selected ? extensions_col::selectable_selected : extensions_col::selectable));
    theme::push_col(ImGuiCol_HeaderHovered, theme::get_col(extensions_col::selectable_hovered));
    theme::push_col(ImGuiCol_HeaderActive,  theme::get_col(extensions_col::selectable_activated));

    bool retval = ImGui::Selectable(label, selected, flags, size);

    theme::pop_col(3);
    return retval;
}

void dv_util_imgui::help_marker(const char* str) {
    ImGui::TextDisabled("(?)");

    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(str);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
