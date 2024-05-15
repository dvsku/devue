#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
    class dv_comp_dockspace : public dv_comp {
    public:
        dv_comp_dockspace() = delete;
    	dv_comp_dockspace(dv_systems* systems, dv_components* components);

    public:
        void render();
    };
}