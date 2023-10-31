#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
	struct dv_comp_console : public dv_comp {
		void render() final override;
	};
}