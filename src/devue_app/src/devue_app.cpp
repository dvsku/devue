#include "gui/dv_gui.hpp"
#include "utilities/dv_util_log.hpp"

using namespace devue;
using namespace devue::core;

int main() {
    dv_util_log::add_source(&std::cout);

    dv_gui gui(1024, 768, "devue");
    gui.run();
}