#include "dv_app.hpp"

using namespace devue;
using namespace devue::core;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    libutil::log::settings log_settings;
    log_settings.log_to_console = false;
    log_settings.level          = libutil::log::level::debug;

    libutil::log::init(log_settings);

    libgui::window_settings settings;
    settings.width                 = 1360U;
    settings.height                = 768U;
    settings.title                 = "devue";
    settings.enable_docking        = true;
    settings.enable_multi_viewport = false;

    dv_app gui(settings);
    gui.show();
}