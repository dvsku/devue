#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
	struct dv_comp_properties : public dv_comp {
		void render() final override;
	};
}