#include "graphics/dv_window.hpp"
#include "dv_gui.hpp"

using namespace devue;
using namespace devue::core;
using namespace devue::gui;

int main() {
	dv_window window(1024, 768, "devue");
	dv_gui gui(window.get_native());
	
	window.attach_gui(gui);
	window.run();

	return 0;
}