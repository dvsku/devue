#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
	struct dv_components;
	
	struct dv_comp_dockspace : public dv_comp {
		dv_comp_dockspace(dv_components* components);

		void render() final override;

	private:
		dv_components* m_components = nullptr;
	};
}