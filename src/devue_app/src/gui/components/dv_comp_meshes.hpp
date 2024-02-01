#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
    struct dv_comp_meshes : public dv_comp {
        dv_comp_meshes(dv_systems* systems, dv_components* components);

        bool render() final override;
    };
}