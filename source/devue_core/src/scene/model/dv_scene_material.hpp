#pragma once

#include <libutil.hpp>

namespace devue::core {
    struct dv_scene_material {
        libutil::uuid diffuse_texture_uuid = 0U;
    };
}