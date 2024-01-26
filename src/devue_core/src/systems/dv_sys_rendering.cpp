#include "systems/dv_sys_rendering.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "exceptions/dv_exception.hpp"
#include "utilities/dv_util_string.hpp"

#include "glad/glad.h"

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
    m_shaders[shader_type::solid]   = create_shader("resources/shaders/solid_color.vert", "resources/shaders/solid_color.frag");
    m_shaders[shader_type::texture] = create_shader("resources/shaders/texture.vert", "resources/shaders/texture.frag");
    m_shaders[shader_type::grid]    = create_shader("resources/shaders/grid.vert", "resources/shaders/grid.frag");

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
    if (smodel.meshes.size())                                 return;

    dv_model& model = m_systems->model.models[smodel.model_uuid];

    for (auto& kvp : model.meshes) {
        dv_mesh& mesh = kvp.second;

        smodel.meshes.push_back({});
        prepare_model(smodel.meshes.back(), mesh, model.skeleton.bones.size() > 0);
    }

    m_systems->material.prepare_model_materials(model);
}

void dv_sys_rendering::prepare_model(dv_scene_mesh& smesh, dv_mesh& mesh, bool is_static) {
    smesh.name = mesh.name;
    smesh.material_uuid = mesh.material_uuid;

    glGenVertexArrays(1, &smesh.vao);

    glBindVertexArray(smesh.vao);

    // Generate vbo
    glGenBuffers(1, &smesh.vbo);

    // Bind vbo for setting up
    glBindBuffer(GL_ARRAY_BUFFER, smesh.vbo);

    // Set vertex data
    // If models have bones, then their vertex positions are gonna change
    // according to the bone matrix.
    // Else they are static and their vertex positions don't change.
    glBufferData(GL_ARRAY_BUFFER,
                 mesh.vertices.size() * sizeof(dv_vertex),
                 &mesh.vertices[0],
                 is_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

    // Generate ibo
    glGenBuffers(1, &smesh.ibo);

    // Bind ibo for setting up
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smesh.ibo);

    // Set index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mesh.faces.size() * sizeof(glm::u16vec3),
                 &mesh.faces[0],
                 GL_STATIC_DRAW);

    smesh.face_count = mesh.faces.size();

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(dv_vertex), (void*)offsetof(dv_vertex, position));

    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(dv_vertex), (void*)offsetof(dv_vertex, normal));

    // UV attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(dv_vertex), (void*)offsetof(dv_vertex, uv));
}

void dv_sys_rendering::release_model(dv_scene_model& smodel) {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    for (auto& smesh : smodel.meshes) {
        glDeleteVertexArrays(1, &smesh.vao);
        glDeleteBuffers(1, &smesh.vbo);
        glDeleteBuffers(1, &smesh.ibo);

        smesh.vao = 0U;
        smesh.vbo = 0U;
        smesh.ibo = 0U;
    }
}

void dv_sys_rendering::render(dv_scene_model& smodel, dv_camera& camera, dv_lighting& lighting) {
    glm::mat4 mvp = camera.get_proj_matrix() * camera.get_view_matrix() * smodel.transform.get_transform_matrix();
    glm::mat3 normal_matrix = smodel.transform.get_normal_matrix();

    for (dv_scene_mesh& smesh : smodel.meshes) {
        if (!smesh.vao || !smesh.vbo || !smesh.ibo) continue;

        const dv_scene_material* material = m_systems->material.get_material(smesh.material_uuid);

        if (material && material->diffuse_texture_uuid && !smodel.wireframe)
            render_textured(material, smesh, camera, lighting, mvp, normal_matrix);
        else
            render_solid(smodel, smesh, camera, lighting, mvp, normal_matrix);
    }
}

void dv_sys_rendering::render(dv_scene_grid& sgrid, dv_camera& camera, dv_lighting& lighting) {
    for (dv_scene_mesh& smesh : sgrid.meshes) {
        if (!smesh.vao || !smesh.vbo || !smesh.ibo) continue;

        render_grid(sgrid, smesh, camera);
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

void dv_sys_rendering::render_solid(dv_scene_model& smodel, dv_scene_mesh& smesh, 
    								dv_camera& camera, dv_lighting& lighting, glm::mat4& mvp, glm::mat3& nm) 
{
    auto& shader = m_shaders[shader_type::solid];

    shader.use();
    shader.set("mvp",               mvp);
    shader.set("normal_mat",        nm);
    shader.set("object_color",      glm::vec3(1.0f, 1.0f, 1.0f));
    shader.set("ambient_light",     lighting.ambient_light.get_light_color());
    shader.set("dir_light_dir",     lighting.directional_light.direction);
    shader.set("dir_light_color",   lighting.directional_light.get_light_color());

    glBindVertexArray(smesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, smesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smesh.ibo);
    glDrawElements(GL_TRIANGLES, (GLsizei)smesh.face_count * 3, GL_UNSIGNED_SHORT, 0);

    if (smodel.wireframe) {
        // Set wire colors
        shader.set("object_color",      glm::vec3(0.5f, 0.5f, 0.5f));
        shader.set("ambient_light",     glm::vec3(1.0f, 1.0f, 1.0f));
        shader.set("dir_light_color",   glm::vec3(0.0f, 0.0f, 0.0f));

        // Enable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Offset lines a bit outside the model so they don't overlap
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-0.5, -0.5);

        // Draw wireframe
        glDrawElements(GL_TRIANGLES, (GLsizei)smesh.face_count * 3, GL_UNSIGNED_SHORT, 0);

        // Disable offset
        glDisable(GL_POLYGON_OFFSET_FILL);

        // Disable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void dv_sys_rendering::render_textured(const dv_scene_material* smaterial, dv_scene_mesh& smesh, 
    								   dv_camera& camera, dv_lighting& lighting, glm::mat4& mvp, glm::mat3& nm) 
{
    auto& shader = m_shaders[shader_type::texture];

    shader.use();
    shader.set("mvp",               mvp);
    shader.set("normal_mat",        nm);
    shader.set("ambient_light",     lighting.ambient_light.get_light_color());
    shader.set("dir_light_dir",     lighting.directional_light.direction);
    shader.set("dir_light_color",   lighting.directional_light.get_light_color());

    auto diffuse_texture = m_systems->texture.get_texture(smaterial->diffuse_texture_uuid);
    if (diffuse_texture && diffuse_texture->texture_id)
        glBindTexture(GL_TEXTURE_2D, diffuse_texture->texture_id);

    glBindVertexArray(smesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, smesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smesh.ibo);
    glDrawElements(GL_TRIANGLES, (GLsizei)smesh.face_count * 3, GL_UNSIGNED_SHORT, 0);
}

void dv_sys_rendering::render_grid(dv_scene_model& smodel, dv_scene_mesh& smesh, dv_camera& camera) 
{
    auto& shader = m_shaders[shader_type::grid];

    shader.use();
    shader.set("mat_view", camera.get_view_matrix());
    shader.set("mat_proj", camera.get_proj_matrix());
    shader.set("mat_mod",  smodel.transform.get_transform_matrix());

    glBindVertexArray(smesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, smesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smesh.ibo);
    glDrawElements(GL_TRIANGLES, (GLsizei)smesh.face_count * 3, GL_UNSIGNED_SHORT, 0);
}