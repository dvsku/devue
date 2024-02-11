#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "systems/dv_systems.hpp"

namespace devue {
    struct dv_components;

    struct dv_comp {
    	dv_comp() = delete;
    	dv_comp(dv_systems* systems, dv_components* components);
    	
    	virtual bool render() = 0;
        void focus();

    protected:
    	dv_systems*    m_systems    = nullptr;
    	dv_components* m_components = nullptr;

        bool m_requested_focus = false;
    };
}