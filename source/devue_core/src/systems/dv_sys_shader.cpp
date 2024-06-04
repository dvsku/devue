#include "systems/dv_sys_shader.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "exceptions/dv_exception.hpp"

#include <glad/glad.h>
#include <cmrc/cmrc.hpp>

CMRC_DECLARE(resources);

using namespace devue::core;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_sys_shader::dv_sys_shader(dv_systems_bundle* systems)
    : m_systems(systems) {}

bool dv_sys_shader::prepare() {
    if (!compile_integrated_shaders())
        return false;

    if (!compile_user_shaders())
        return false;

    return true;
}

void dv_sys_shader::release() {}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

bool dv_sys_shader::compile_integrated_shaders() {
    auto fs = cmrc::resources::get_filesystem();

    {
        if (!fs.exists("resources/shaders/default.vert") || !fs.exists("resources/shaders/default.frag"))
            return false;

        auto file_vert = fs.open("resources/shaders/default.vert");
        auto file_frag = fs.open("resources/shaders/default.frag");

        std::string source_vert = std::string(file_vert.cbegin(), file_vert.cend());
        std::string source_frag = std::string(file_frag.cbegin(), file_frag.cend());

        auto uuid = dvsku::util_uuid::create("integrated_default");

        shaders[uuid] = compile_shader(source_vert, source_frag);
    }

    {
        if (!fs.exists("resources/shaders/grid.vert") || !fs.exists("resources/shaders/grid.frag"))
            return false;

        auto file_vert = fs.open("resources/shaders/grid.vert");
        auto file_frag = fs.open("resources/shaders/grid.frag");

        std::string source_vert = std::string(file_vert.cbegin(), file_vert.cend());
        std::string source_frag = std::string(file_frag.cbegin(), file_frag.cend());

        auto uuid = dvsku::util_uuid::create("integrated_grid");

        shaders[uuid] = compile_shader(source_vert, source_frag);
    }

    return true;
}

bool dv_sys_shader::compile_user_shaders() {
    return true;
}

dv_shader dv_sys_shader::compile_shader(const std::string& vertex_code, const std::string& fragment_code) {
    const char* vertex_source   = vertex_code.c_str();
    const char* fragment_source = fragment_code.c_str();

    dv_shader   shader{};
    int         status        = 0;
    uint32_t    vertex_id     = 0U;
    uint32_t    fragment_id   = 0U;
    int         error_msg_len = 0;
    std::string error_msg     = "";

    vertex_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id, 1, &vertex_source, NULL);
    glCompileShader(vertex_id);

    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &error_msg_len);

        error_msg.resize(error_msg_len);
        glGetShaderInfoLog(vertex_id, error_msg_len, NULL, error_msg.data());

        throw dv_exception(DV_FORMAT("Failed to compile vertex shader : {}", error_msg));
    }

    fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &fragment_source, NULL);
    glCompileShader(fragment_id);

    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &error_msg_len);

        error_msg.resize(error_msg_len);
        glGetShaderInfoLog(fragment_id, error_msg_len, NULL, error_msg.data());

        throw dv_exception(DV_FORMAT("Failed to compile fragment shader : {}", error_msg));
    }

    shader.shader_id = glCreateProgram();
    glAttachShader(shader.shader_id, vertex_id);
    glAttachShader(shader.shader_id, fragment_id);
    glLinkProgram(shader.shader_id);

    glGetProgramiv(shader.shader_id, GL_LINK_STATUS, &status);
    if (!status) {
        glGetShaderiv(shader.shader_id, GL_INFO_LOG_LENGTH, &error_msg_len);

        error_msg.resize(error_msg_len);
        glGetShaderInfoLog(shader.shader_id, error_msg_len, NULL, error_msg.data());

        throw dv_exception(DV_FORMAT("Failed to link shader : {}", error_msg));
    }

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

    return shader;
}
