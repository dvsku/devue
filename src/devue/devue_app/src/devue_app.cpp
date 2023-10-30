#include "dv_window.hpp"

using namespace devue;
using namespace devue::core;

int main() {
	dv_window window(1024, 768, "devue");
	window.run();
}