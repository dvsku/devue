#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <unordered_map>

namespace devue {
    class dv_util_imgui {
    public:
        static bool collapsible(const char* label, ImGuiTreeNodeFlags flags = 0);

        static bool selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));

        static void help_marker(const char* str);
    };
}