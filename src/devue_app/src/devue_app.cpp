#include "gui/dv_gui.hpp"

using namespace devue;
using namespace devue::core;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
    dv_gui gui(1360, 768, "devue");
    gui.run();
}