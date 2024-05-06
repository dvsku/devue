#include "gui/dv_gui.hpp"

using namespace devue;
using namespace devue::core;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    dvsku::dv_util_log::settings log_settings;
    log_settings.level = dvsku::dv_util_log::level::debug;

    dvsku::dv_util_log::init(log_settings);

    dvsku::dv_window_settings settings;
    settings.width                 = 1360U;
    settings.height                = 768U;
    settings.title                 = "devue";
    settings.enable_docking        = true;
    settings.enable_multi_viewport = false;

    dv_gui gui(settings);
    gui.show();
}