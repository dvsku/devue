#pragma once

#include "gui/base/dv_comp.hpp"
#include "systems/dv_systems.hpp"

namespace devue {
    struct dv_comp_assets : public dv_comp {
    	dv_comp_assets(dv_systems* systems, dv_components* components);

    	bool render() final override;
    };
}