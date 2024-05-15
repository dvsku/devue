#pragma once

#include "gui/base/dv_comp.hpp"

#include <sstream>

namespace devue {
    class dv_comp_console : public dv_comp {
    public:
        dv_comp_console() = delete;
    	dv_comp_console(dv_systems* systems, dv_components* components);
        ~dv_comp_console();
    
    public:
        dvsku::dv_command_state render();

    private:
        std::stringstream m_text;
    };
}