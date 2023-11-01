#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "misc/cpp/imgui_stdlib.h"

#include "systems/dv_systems.hpp"

namespace devue {
	struct dv_components;

	struct dv_comp {
		bool visible = true;

		dv_comp() = delete;
		dv_comp(dv_systems* systems, dv_components* components);
		
		virtual void render() = 0;

	protected:
		dv_systems* m_systems = nullptr;
		dv_components* m_components = nullptr;
	};
}