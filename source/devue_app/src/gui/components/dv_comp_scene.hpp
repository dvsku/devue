#pragma once

#include "gui/base/dv_comp.hpp"
#include "rendering/dv_render_target.hpp"

namespace devue {
    class dv_comp_scene : public dv_comp {
    public:
    	bool is_hovered = false;

    public:
        dv_comp_scene() = delete;
    	dv_comp_scene(dv_systems* systems, dv_components* components);

    public:
        void render(core::dv_render_target* render_target = nullptr);

    private:
        void render();
    };
}