#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
    struct dv_comp_dockspace : public dv_comp {
    	dv_comp_dockspace(dv_systems* systems, dv_components* components);

        bool render() final override;
    };
}