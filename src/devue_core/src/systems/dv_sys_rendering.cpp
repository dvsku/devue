#include "exceptions/dv_exception.hpp"
#include "systems/dv_sys_rendering.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "utilities/dv_util_string.hpp"

#include "glad/glad.h"

#include <filesystem>
#include <fstream>

using namespace devue::core;

enum integrated_shader : uint8_t {
    def  = 0x1,
    grid = 0x2
};

dv_sys_rendering::dv_sys_rendering(dv_systems_bundle* systems)
    : m_systems(systems) {}

void dv_sys_rendering::prepare() {
    m_shaders[integrated_shader::def]  = create_shader("resources/shaders/default.vert", "resources/shaders/default.frag");
    m_shaders[integrated_shader::grid] = create_shader("resources/shaders/grid.vert",    "resources/shaders/grid.frag");

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void dv_sys_rendering::prepare_model(dv_scene_model& smodel) {
    if (!m_systems->model.models.contains(smodel.model_uuid)) return;
    prepare_model(smodel, m_systems->model.models[smodel.model_uuid]);
}

void dv_sys_rendering::prepare_model(dv_scene_model& smodel, dv_model& model) {
    if (smodel.meshes.size()) return;

    // Generate vao
    glGenVertexArrays(1, &smodel.vao);

    // Bind vao
    glBindVertexArray(smodel.vao);

    // Generate vbo
    glGenBuffers(1, &smodel.vbo);

    // Bind vbo for setting up
    glBindBuffer(GL_ARRAY_BUFFER, smodel.vbo);

    // Set vertex data
    // If models have bones, then their vertex positions are gonna change
    // according to the bone matrix.
    // Else they are static and their vertex positions don't change.
    glBufferData(GL_ARRAY_BUFFER,
        model.vertices.size() * sizeof(dv_vertex),
        &model.vertices[0],
        model.skeleton.bones.size() > 0 ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

    for (auto& kvp : model.meshes) {
        dv_mesh& mesh = kvp.second;

        smodel.meshes.push_back({});
        prepare_model(smodel.meshes.back(), mesh, model.skeleton.bones.size() > 0);
    }

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(dv_vertex), (void*)offsetof(dv_vertex, position));

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(dv_vertex), (void*)offsetof(dv_vertex, normal));

    // UV attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(dv_vertex), (void*)offsetof(dv_vertex, uv));

    m_systems->material.prepare_model_materials(model);
}

void dv_sys_rendering::prepare_model(dv_scene_mesh& smesh, dv_mesh& mesh, bool is_static) {
    smesh.name          = mesh.name;
    smesh.material_uuid = mesh.material_uuid;
    smesh.face_count    = mesh.faces.size();

    // Generate ibo
    glGenBuffers(1, &smesh.ibo);

    // Bind ibo for setting up
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smesh.ibo);

    // Set index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    smesh.face_count * sizeof(glm::u32vec3),
                    &mesh.faces[0],
                    GL_STATIC_DRAW);
}

void dv_sys_rendering::release_model(dv_scene_model& smodel) {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    for (auto& smesh : smodel.meshes) {
        glDeleteBuffers(1, &smesh.ibo);

        smesh.ibo = 0U;
    }

    glDeleteVertexArrays(1, &smodel.vao);
    glDeleteBuffers(1, &smodel.vbo);

    smodel.vao = 0U;
    smodel.vbo = 0U;
}

void dv_sys_rendering::render(dv_scene_model& smodel, dv_camera& camera, dv_lighting& lighting) {
    if (!smodel.vao || !smodel.vbo)                           return;
    if (!m_systems->model.models.contains(smodel.model_uuid)) return;

    dv_shader* shader = set_shader(integrated_shader::def);
    if (!shader) return;

    const dv_model& model        = m_systems->model.models[smodel.model_uuid];
    glm::mat4       model_matrix = smodel.transform.get_transform_matrix();

    // Apply model modifications
    if (model.flag_z_up_to_y_up && model.flag_lh_to_rh) {
        model_matrix = glm::rotate(model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model_matrix = glm::rotate(model_matrix, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    else if (model.flag_z_up_to_y_up) {
        model_matrix = glm::rotate(model_matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    else if (model.flag_lh_to_rh) {
        model_matrix = glm::rotate(model_matrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat3 normal_matrix {
        glm::normalize(glm::vec3(model_matrix[0])),
        glm::normalize(glm::vec3(model_matrix[1])),
        glm::normalize(glm::vec3(model_matrix[2]))
    };
    normal_matrix = glm::transpose(glm::inverse(normal_matrix));

    glm::mat4 mvp       = camera.get_proj_matrix() * camera.get_view_matrix() * model_matrix;
    bool      wireframe = smodel.wireframe;

    shader->set("uf_mvp",        mvp);
    shader->set("uf_normal_mat", normal_matrix);
    shader->set("uf_dl_dir",     lighting.directional_light.direction);
    
    glBindVertexArray(smodel.vao);
    glBindBuffer(GL_ARRAY_BUFFER, smodel.vbo);

    for (dv_scene_mesh& smesh : smodel.meshes) {
        if (!smesh.ibo) continue;

        const dv_scene_material* material        = nullptr;
        const dv_scene_texture*  diffuse_texture = nullptr;   
        bool                     textured        = false;

        if (!wireframe) {
            material        = m_systems->material.get_material(smesh.material_uuid);
            diffuse_texture = material ? m_systems->texture.get_texture(material->diffuse_texture_uuid) : nullptr;
            textured        = diffuse_texture && diffuse_texture->texture_id;
        }

        shader->set("uf_textured", textured ? 1.0f : 0.0f);
        shader->set("uf_col",      glm::vec3(1.0f, 1.0f, 1.0f));
        shader->set("uf_al_col",   wireframe ? glm::vec3(1.0f, 1.0f, 1.0f) : lighting.ambient_light.get_light_color());
        shader->set("uf_dl_col",   wireframe ? glm::vec3(0.0f, 0.0f, 0.0f) : lighting.directional_light.get_light_color());

        if (textured)
            glBindTexture(GL_TEXTURE_2D, diffuse_texture->texture_id);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smesh.ibo);
        glDrawElements(GL_TRIANGLES, (GLsizei)smesh.face_count * 3, GL_UNSIGNED_INT, 0);

        if (wireframe) {
            // Set wire colors
            shader->set("uf_col",    glm::vec3(0.5f, 0.5f, 0.5f));
            shader->set("uf_al_col", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->set("uf_dl_col", glm::vec3(0.0f, 0.0f, 0.0f));

            // Enable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            // Offset lines a bit outside the model so they don't overlap
            glEnable(GL_POLYGON_OFFSET_LINE);
            glPolygonOffset(-0.5, -0.5);

            // Draw wireframe
            glDrawElements(GL_TRIANGLES, (GLsizei)smesh.face_count * 3, GL_UNSIGNED_INT, 0);

            // Disable offset
            glDisable(GL_POLYGON_OFFSET_FILL);

            // Disable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

void dv_sys_rendering::render(dv_scene_grid& sgrid, dv_camera& camera, dv_lighting& lighting) {
    if (!sgrid.vao || !sgrid.vbo) return;

    // Set grid shader
    dv_shader* shader = set_shader(integrated_shader::grid);
    if (!shader) return;

    shader->set("uf_model_mat", sgrid.transform.get_transform_matrix());
    shader->set("uf_view_mat",  camera.get_view_matrix());
    shader->set("uf_proj_mat",  camera.get_proj_matrix());

    glBindVertexArray(sgrid.vao);
    glBindBuffer(GL_ARRAY_BUFFER, sgrid.vbo);

    for (dv_scene_mesh& smesh : sgrid.meshes) {
        if (!smesh.ibo) continue;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smesh.ibo);
        glDrawElements(GL_TRIANGLES, (GLsizei)smesh.face_count * 3, GL_UNSIGNED_INT, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

std::string dv_sys_rendering::get_shader_source(const std::string& path) {
    std::filesystem::path filepath(path);

    if (path.empty())
        throw dv_exception("Failed to load shader source. File path empty.");

    if (!std::filesystem::exists(filepath))
        throw dv_exception(DV_FORMAT("Failed to load shader source. File `{}` not found.", path));
    
    std::string source = "";
    std::ifstream file(filepath);

    if (!file.is_open())
        throw dv_exception(DV_FORMAT("Failed to load shader source. Couldn't open `{}` file.", path));

    try {
        std::string line;
        while (std::getline(file, line)) {
            source += line + '\n';
        }
        file.close();
    }
    catch (const std::exception& e) {
        throw dv_exception(DV_FORMAT("Failed to load shader source. | {}", e.what()));
    }
    catch (...) {
        throw dv_exception("Failed to load shader source.");
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

        throw dv_exception(DV_FORMAT("Failed to compile vertex shader : {}", error_msg));
    }

    fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_id, 1, &fragment_source_c, NULL);
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

dv_shader* dv_sys_rendering::set_shader(uint8_t shader_id) {
    if (m_current_shader_id == shader_id) return &m_shaders[m_current_shader_id];
    if (!m_shaders.contains(shader_id))   return nullptr;

    m_current_shader_id = shader_id;
    m_shaders[shader_id].use();

    return &m_shaders[shader_id];
}