#pragma once

#include "gui/base/dv_selected.hpp"
#include "gui/components/dv_comp_assets.hpp"
#include "gui/components/dv_comp_console.hpp"
#include "gui/components/dv_comp_dockspace.hpp"
#include "gui/components/dv_comp_hierarchy.hpp"
#include "gui/components/dv_comp_properties.hpp"
#include "gui/components/dv_comp_scene.hpp"
#include "systems/dv_systems.hpp"

namespace devue {
	struct dv_components {
		dv_comp_assets		assets;
		dv_comp_console		console;
		dv_comp_dockspace	dockspace;
		dv_comp_hierarchy	hierarchy;
		dv_comp_properties	properties;
		dv_comp_scene		scene;

		dv_selected selected{};

		dv_components() = delete;
		dv_components(dv_systems* systems);
	};
}