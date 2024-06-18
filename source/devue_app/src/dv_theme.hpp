#pragma once

namespace devue {
    enum class extensions_col {
        collapsible,
        collapsible_hovered,
        collapsible_activated,
        selectable,
        selectable_hovered,
        selectable_activated,
        selectable_selected
    };

    /*
        Add custom theme colors to libgui theme.
        
        FUTURE:
            Load theme colors from file and set libgui theme and custom theme colors
            from a file.
    */

    class dv_theme {
    public:
        static void set();
    };
}
