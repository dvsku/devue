#include "dv_theme.hpp"

#include <libgui/theme.hpp>

using namespace devue;
using namespace libgui;

void dv_theme::set() {
    /*
        Set fonts
    */

    libgui::theme::add_font("default", 13.0f, {
        font_average_mono,
        font_fontawesome_solid,
        font_fontawesome_regular
    });

    /*
        Set custom theme colors
    */

    theme::set_col(extensions_col::collapsible,           { 0.29020f, 0.24314f, 0.61176f, 1.00f });
    theme::set_col(extensions_col::collapsible_hovered,   lighten(theme::get_col(extensions_col::collapsible)));
    theme::set_col(extensions_col::collapsible_activated, darken(theme::get_col(extensions_col::collapsible)));
    theme::set_col(extensions_col::selectable,            { 0.0f, 0.0f, 0.0f, 0.0f });
    theme::set_col(extensions_col::selectable_hovered,    { 0.23922f, 0.23922f, 0.23922f, 1.00f });
    theme::set_col(extensions_col::selectable_activated,  theme::get_col(extensions_col::selectable_hovered));
    theme::set_col(extensions_col::selectable_selected,   theme::get_col(extensions_col::selectable_hovered));
}
