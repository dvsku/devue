#include "gui/dv_gui.hpp"

using namespace devue;
using namespace devue::core;

int main() {
	dv_gui gui(1024, 768, "devue");
	gui.run();
}