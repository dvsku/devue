#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
	struct dv_comp_scene : public dv_comp {
		dv_comp_scene(dv_systems* systems, dv_components* components);

		void render() final override;
	};
}