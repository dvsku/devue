#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
    struct dv_comp_hierarchy : public dv_comp {
    	dv_comp_hierarchy(dv_systems* systems, dv_components* components);

        bool render() final override;

    private:
        void render_scene_model_context_menu(core::dv_scene_model& smodel);
    };
}