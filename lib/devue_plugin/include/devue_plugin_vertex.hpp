#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtx/hash.hpp"

namespace devue::plugins {
    class devue_plugin_vertex {
    public:
        glm::vec3 position{};
        glm::vec3 normal{};
        glm::vec2 uv{};

    public:
        bool operator==(const devue_plugin_vertex& rhs) const {
            return position == rhs.position && normal == rhs.normal && uv == rhs.uv;
        }
    };
}

namespace std {
    template<> struct hash<devue::plugins::devue_plugin_vertex> {
        size_t operator()(devue::plugins::devue_plugin_vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^ 
                (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.uv) << 1);
        }
    };
}