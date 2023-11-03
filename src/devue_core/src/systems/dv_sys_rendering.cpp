#include "glad/glad.h"
#include "systems/dv_sys_rendering.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "exceptions/dv_exception.hpp"
#include "utilities/dv_util_string.hpp"

#include <filesystem>
#include <fstream>

using namespace devue::core;

enum shader_type : uint8_t {
    solid,
    texture,
    grid
};

dv_sys_rendering::dv_sys_rendering(dv_systems_bundle* systems)
	: m_systems(systems) {}

void dv_sys_rendering::prepare() {
    m_shaders[shader_type::solid]   = create_shader("", "");
    m_shaders[shader_type::texture] = create_shader("", "");
    m_shaders[shader_type::grid]    = create_shader("", "");
}

void dv_sys_rendering::prepare_model(dv_scene_model& smodel) {}

void dv_sys_rendering::prepare_model(dv_scene_model& smesh, dv_mesh& mesh, bool is_static) {}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

std::string dv_sys_rendering::get_shader_source(const std::string& path) {
    std::filesystem::path filepath(path);

    if (path.empty())
        throw DV_EXCEPTION("Failed to load shader source. File path empty.");

    if (!std::filesystem::exists(filepath))
        throw DV_EXCEPTION(DV_FORMAT("Failed to load shader source. File `{}` not found.", path));
    
    std::string source = "";
    std::ifstream file(filepath);

    if (!file.is_open())
        throw DV_EXCEPTION(DV_FORMAT("Failed to load shader source. Couldn't open `{}` file.", path));

    try {
        std::string line;
        while (std::getline(file, line)) {
            source += line;
        }
        file.close();
    }
    catch (const std::exception& e) {
        throw DV_EXCEPTION(DV_FORMAT("Failed to load shader source. | {}", e.what()));
    }
    catch (...) {
        throw DV_EXCEPTION("Failed to load shader source.");
    }

    return source;
}

dv_shader dv_sys_rendering::create_shader(const std::string& vertex, const std::string& fragment) {
    std::string vertex_source   = get_shader_source(vertex);
    std::string fragment_source = get_shader_source(fragment);
    
    const char* vertex_source_c   = vertex_source.c_str();
    const char* fragment_source_c = fragment_source.c_str();

    dv_shader shader{};

    int status           = 0;
    uint32_t vertex_id   = 0U;
    uint32_t fragment_id = 0U;

    int error_msg_len     = 0;
    std::string error_msg = "";

    vertex_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_id, 1, &vertex_source_c, NULL);
    glCompileShader(vertex_id);

    glGetShaderiv(vertex_id, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderiv(vertex_id, GL_INFO_LOG_LENGTH, &error_msg_len);
        
        error_msg.resize(error_msg_len);
        glGetShaderInfoLog(vertex_id, error_msg_len, NULL, error_msg.data());

        throw DV_EXCEPTION(DV_FORMAT("Failed to compile vertex shader : {}", error_msg));
    }

    fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &fragment_source_c, NULL);
    glCompileShader(fragment_id);

    glGetShaderiv(fragment_id, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderiv(fragment_id, GL_INFO_LOG_LENGTH, &error_msg_len);

        error_msg.resize(error_msg_len);
        glGetShaderInfoLog(fragment_id, error_msg_len, NULL, error_msg.data());

        throw DV_EXCEPTION(DV_FORMAT("Failed to compile fragment shader : {}", error_msg));
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

        throw DV_EXCEPTION(DV_FORMAT("Failed to link shader : {}", error_msg));
    }

    glDeleteShader(vertex_id);
    glDeleteShader(fragment_id);

	return shader;
}

void dv_sys_rendering::render_solid(dv_scene_model& smodel, dv_scene_mesh& smesh, 
									dv_camera& camera, dv_lighting& lighting, glm::mat4& mvp, glm::mat3& nm) 
{

}

void dv_sys_rendering::render_textured(const dv_scene_material* smaterial, dv_scene_mesh& smesh, 
									   dv_camera& camera, dv_lighting& lighting, glm::mat4& mvp, glm::mat3& nm) 
{

}

void dv_sys_rendering::render_grid(dv_scene_model& smodel, dv_scene_mesh& smesh, dv_camera& camera) 
{

}