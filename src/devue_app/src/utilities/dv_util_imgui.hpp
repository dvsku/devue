#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include <unordered_map>

namespace devue {
    class dv_util_imgui {
    public:
        static void init();

    public:
        static bool collapsable(const char* label, ImGuiTreeNodeFlags flags = 0);

        static bool selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

        static bool begin_menu(const char* label, bool enabled = true);
        static bool begin_item_context_menu(const char* str_id = nullptr, ImGuiPopupFlags popup_flags = 1);

        static void help_marker(const char* str);

        static bool link(const char* str);

        static bool icon_button(const char* label, const ImVec2& size = ImVec2(0, 0));

    private:
        enum class widget_color : uint16_t {
            collapsable,
            collapsable_hovered,
            collapsable_activated,
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