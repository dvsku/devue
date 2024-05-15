#pragma once

#include "gui/base/dv_comp.hpp"
#include "systems/dv_systems.hpp"

namespace devue {
    class dv_comp_assets : public dv_comp {
    public:
        dv_comp_assets() = delete;
    	dv_comp_assets(dv_systems* systems, dv_components* components);

    public:
    	void render();

    private:
        void render_context_menu(core::dv_model& model);
    };
}