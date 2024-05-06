#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
    class dv_comp_hierarchy : public dv_comp {
    public:
        dv_comp_hierarchy() = delete;
    	dv_comp_hierarchy(dv_systems* systems, dv_components* components);

    public:
        void render();

    private:
        void render_scene_model_context_menu(core::dv_scene_model& smodel);
    };
}