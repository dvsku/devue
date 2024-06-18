#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <unordered_map>

namespace devue {
    class dv_util_imgui {
    public:
        static void init();

    public:
        static bool collapsible(const char* label, ImGuiTreeNodeFlags flags = 0);

        static bool selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

        static void help_marker(const char* str);

    private:
        enum class widget_color : uint16_t {
            collapsible,
            collapsible_hovered,
            collapsible_activated,
            selectable,
            selectable_hovered,
            selectable_activated,
            selectable_selected,
            link,
            link_hovered,
            link_activated,
            icon_button,
            icon_button_hovered,
            icon_button_activated,
            icon_button_text
        };

    private:
        inline static std::unordered_map<widget_color, ImVec4> m_colors;
    };
}