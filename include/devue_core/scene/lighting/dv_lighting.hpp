#pragma once

#include "scene/lighting/dv_ambient_light.hpp"
#include "scene/lighting/dv_directional_light.hpp"

namespace devue::core {
	struct dv_lighting {
		dv_ambient_light	 ambient_light{};
		dv_directional_light directional_light{};
	};
}