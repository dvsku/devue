#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
    class dv_comp_properties : public dv_comp {
    public:
        dv_comp_properties() = delete;
    	dv_comp_properties(dv_systems* systems, dv_components* components);

    public:
        void render();

    private:
        void render_model();
    	void render_scene_model();
    	void render_camera();
    	void render_ambient_light();
    	void render_directional_light();
    };
}