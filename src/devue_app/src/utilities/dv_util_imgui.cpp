#include "utilities/dv_util_imgui.hpp"

using namespace devue;

void dv_util_imgui::init() {
    m_colors[widget_color::collapsable]           = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    m_colors[widget_color::collapsable_hovered]   = ImLerp(m_colors[widget_color::collapsable], ImVec4(1.0f, 1.0f, 1.0f, 1.00f), 0.1f);
    m_colors[widget_color::collapsable_activated] = ImLerp(m_colors[widget_color::collapsable], ImVec4(0.0f, 0.0f, 0.0f, 1.00f), 0.2f);

    m_colors[widget_color::selectable]            = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    m_colors[widget_color::selectable_hovered]    = ImVec4(0.23922f, 0.23922f, 0.23922f, 1.00f);
    m_colors[widget_color::selectable_activated]  = ImVec4(0.23922f, 0.23922f, 0.23922f, 1.00f);
    m_colors[widget_color::selectable_selected]   = ImVec4(0.23922f, 0.23922f, 0.23922f, 1.00f);
}

bool dv_util_imgui::collapsable(const char* label, ImGuiTreeNodeFlags flags) {
    ImGui::PushStyleColor(ImGuiCol_Header,        m_colors[widget_color::collapsable]);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, m_colors[widget_color::collapsable_hovered]);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive,  m_colors[widget_color::collapsable_activated]);

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

bool dv_util_imgui::begin_menu(const char* label, bool enabled) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8.0f, 6.0f });

    ImGui::SetNextWindowSize({ 225.0f, 0.0f });
    bool result = ImGui::BeginMenu(label, enabled);

    ImGui::PopStyleVar(1);

    return result;
}

bool dv_util_imgui::begin_item_context_menu(const char* str_id, ImGuiPopupFlags popup_flags) {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8.0f, 6.0f });

    ImGui::SetNextWindowSize({ 225.0f, 0.0f });
    bool result = ImGui::BeginPopupContextItem(str_id, popup_flags);

    ImGui::PopStyleVar(1);

    return result;
}
