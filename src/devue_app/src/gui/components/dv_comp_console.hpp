#pragma once

#include "gui/base/dv_comp.hpp"

#include <sstream>

namespace devue {
    struct dv_comp_console : public dv_comp {
    	dv_comp_console(dv_systems* systems, dv_components* components);
        ~dv_comp_console();

        bool render() final override;

    private:
        std::stringstream m_text;
    };
}