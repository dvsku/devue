#include "utilities/dv_util_imgui.hpp"

using namespace devue;

void dv_util_imgui::init() {
    m_colors[widget_color::collapsible]           = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    m_colors[widget_color::collapsible_hovered]   = ImLerp(m_colors[widget_color::collapsible], ImVec4(1.0f, 1.0f, 1.0f, 1.00f), 0.1f);
    m_colors[widget_color::collapsible_activated] = ImLerp(m_colors[widget_color::collapsible], ImVec4(0.0f, 0.0f, 0.0f, 1.00f), 0.2f);

    m_colors[widget_color::selectable]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    m_colors[widget_color::selectable_hovered]    = ImVec4(0.23922f, 0.23922f, 0.23922f, 1.00f);
    m_colors[widget_color::selectable_activated]  = ImVec4(0.23922f, 0.23922f, 0.23922f, 1.00f);
    m_colors[widget_color::selectable_selected]   = ImVec4(0.23922f, 0.23922f, 0.23922f, 1.00f);

    m_colors[widget_color::link]                  = ImVec4(0.38824f, 0.32157f, 0.79608f, 1.00f);
    m_colors[widget_color::link_hovered]          = ImLerp(m_colors[widget_color::link], ImVec4(1.0f, 1.0f, 1.0f, 1.00f), 0.1f);
    m_colors[widget_color::link_activated]        = ImLerp(m_colors[widget_color::link], ImVec4(0.0f, 0.0f, 0.0f, 1.00f), 0.2f);

    m_colors[widget_color::icon_button]           = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    m_colors[widget_color::icon_button_hovered]   = ImVec4(0.23922f, 0.23922f, 0.23922f, 1.00f);
    m_colors[widget_color::icon_button_activated] = ImVec4(0.23922f, 0.23922f, 0.23922f, 1.00f);
    m_colors[widget_color::icon_button_text]      = ImVec4(0.77255f, 0.77255f, 0.77255f, 1.00f);
    
}

bool dv_util_imgui::collapsible(const char* label, ImGuiTreeNodeFlags flags) {
    ImGui::PushStyleColor(ImGuiCol_Header,        m_colors[widget_color::collapsible]);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, m_colors[widget_color::collapsible_hovered]);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive,  m_colors[widget_color::collapsible_activated]);

    bool result = ImGui::CollapsingHeader(label, flags);

    ImGui::PopStyleColor(3);

    return result;
}

bool dv_util_imgui::selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size) {
    if (!selected)
        ImGui::PushStyleColor(ImGuiCol_Header, m_colors[widget_color::selectable]);
    else
        ImGui::PushStyleColor(ImGuiCol_Header, m_colors[widget_color::selectable_selected]);

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, m_colors[widget_color::selectable_hovered]);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive,  m_colors[widget_color::selectable_activated]);

    bool result = ImGui::Selectable(label, selected, flags, size);

    ImGui::PopStyleColor(3);

    return result;
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
