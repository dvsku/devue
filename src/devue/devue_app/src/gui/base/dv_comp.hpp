#pragma once

#include "imgui.h"
#include "imgui_internal.h"

namespace devue {
	struct dv_comp {
		bool visible = true;

		virtual void render() = 0;
	};
}