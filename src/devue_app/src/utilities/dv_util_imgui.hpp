#pragma once

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

    private:
        enum class widget_color : uint16_t {
            collapsable,
            collapsable_hovered,
            collapsable_activated,
            selectable,
            selectable_hovered,
            selectable_activated,
            selectable_selected
        };

    private:
        inline static std::unordered_map<widget_color, ImVec4> m_colors;
    };
}