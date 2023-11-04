#pragma once

#include "gui/base/dv_comp.hpp"
#include "rendering/dv_render_target.hpp"

namespace devue {
    struct dv_comp_scene : public dv_comp {
    	bool is_hovered = false;

    	dv_comp_scene(dv_systems* systems, dv_components* components);

    	void render(core::dv_render_target* render_target = nullptr);

    private:
    	void render() final override;
    };
}