#pragma once

#include "scene/model/dv_scene_texture.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <utility>
#include <unordered_map>

namespace devue::core {
    class dv_sys_texture {
    public:

    	size_t count() const;

    public:
    	std::unordered_map<devue::uuid, std::pair<uint16_t, dv_scene_texture>> m_textures;
    };
}