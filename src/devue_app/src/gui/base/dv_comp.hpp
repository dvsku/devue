#pragma once

#include "systems/dv_systems.hpp"

#include <dv_gui_opengl/dv_gui_opengl.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

namespace devue {
    struct dv_components;

    struct dv_comp {
    	dv_comp() = delete;
    	dv_comp(dv_systems* systems, dv_components* components);
    	
        void focus();

    protected:
    	dv_systems*    m_systems    = nullptr;
    	dv_components* m_components = nullptr;

        bool m_requested_focus = false;
    };
}