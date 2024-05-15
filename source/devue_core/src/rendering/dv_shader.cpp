#include "rendering/dv_shader.hpp"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

using namespace devue::core;

void dv_shader::use() {
    glUseProgram(shader_id);
}

void dv_shader::set(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(shader_id, name.c_str()), (int)value);
}

void dv_shader::set(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(shader_id, name.c_str()), value);
}

void dv_shader::set(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(shader_id, name.c_str()), value);
}

void dv_shader::set(const std::string& name, glm::vec3 vec) const {
    glUniform3f(glGetUniformLocation(shader_id, name.c_str()), vec.x, vec.y, vec.z);
}

void dv_shader::set(const std::string& name, glm::ivec3 vec) const {
    glUniform3i(glGetUniformLocation(shader_id, name.c_str()), vec.x, vec.y, vec.z);
}

void dv_shader::set(const std::string& name, glm::vec4 vec) const {
    glUniform4f(glGetUniformLocation(shader_id, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void dv_shader::set(const std::string& name, glm::mat3 mat) const {
    glUniformMatrix3fv(glGetUniformLocation(shader_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void dv_shader::set(const std::string& name, glm::mat4 mat) const {
    glUniformMatrix4fv(glGetUniformLocation(shader_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
