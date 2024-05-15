#pragma once

#include "rendering/dv_shader.hpp"

#include <unordered_map>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_shader {
    public:
        std::unordered_map<uint8_t, dv_shader> shaders;

    public:
        dv_sys_shader()                     = delete;
        dv_sys_shader(const dv_sys_shader&) = delete;
        dv_sys_shader(dv_sys_shader&&)      = delete;

        dv_sys_shader(dv_systems_bundle* systems);

        dv_sys_shader& operator=(const dv_sys_shader&) = delete;
        dv_sys_shader& operator=(dv_sys_shader&&)      = delete;

    public:
        bool prepare();
        void release();

    private:
        dv_systems_bundle* m_systems = nullptr;
    };
}
