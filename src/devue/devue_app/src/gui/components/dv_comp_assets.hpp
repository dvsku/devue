#pragma once

#include "gui/base/dv_comp.hpp"
#include "systems/dv_systems.hpp"

namespace devue {
	struct dv_comp_assets : public dv_comp {
	public:
		dv_comp_assets(dv_systems* systems);

		void render() final override;
	private:
		dv_systems* m_systems = nullptr;
	};
}