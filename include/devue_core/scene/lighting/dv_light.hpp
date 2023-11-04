#pragma once

#include "glm/glm.hpp"
#include "utilities/dv_util_uuid.hpp"

namespace devue::core {
    struct dv_light {
    	devue::uuid uuid = 0U;

    	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
    	float intensity = 1.0f;

    	bool is_enabled = true;

    	dv_light() {
    		uuid = dv_util_uuid::create();
    	}

    	virtual glm::vec3 get_light_color() {
    		return is_enabled ? color * intensity : glm::vec3(0.0f, 0.0f, 0.0f);
    	}
    };
}