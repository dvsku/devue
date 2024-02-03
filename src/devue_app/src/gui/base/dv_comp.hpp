#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

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