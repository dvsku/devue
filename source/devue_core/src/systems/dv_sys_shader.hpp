#pragma once

#include "rendering/dv_shader.hpp"

#include <dv_utilities.hpp>
#include <unordered_map>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_shader {
    public:
        std::unordered_map<dvsku::uuid, dv_shader> shaders;

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

    private:
        bool compile_integrated_shaders();
        bool compile_user_shaders();

        dv_shader compile_shader(const std::string& vertex_code, const std::string& fragment_code);
    };
}
