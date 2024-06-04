#include "exceptions/dv_exception.hpp"
#include "systems/dv_sys_rendering.hpp"
#include "systems/dv_systems_bundle.hpp"

#include <glad/glad.h>
#include <filesystem>
#include <fstream>

using namespace devue::core;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_sys_rendering::dv_sys_rendering(dv_systems_bundle* systems)
    : m_systems(systems) {}

void dv_sys_rendering::prepare() {
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    smesh.mesh_uuid     = mesh.uuid;
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

    static const dvsku::uuid SHADER_ID = dvsku::util_uuid::create("integrated_default");

    dv_shader* shader = set_shader(SHADER_ID);
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

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

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

    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void dv_sys_rendering::render(dv_scene_grid& sgrid, dv_camera& camera, dv_lighting& lighting) {
    if (!sgrid.vao || !sgrid.vbo) return;

    static const dvsku::uuid SHADER_ID = dvsku::util_uuid::create("integrated_grid");

    // Set grid shader
    dv_shader* shader = set_shader(SHADER_ID);
    if (!shader) return;

    shader->set("uf_model_mat", sgrid.transform.get_transform_matrix());
    shader->set("uf_view_mat",  camera.get_view_matrix());
    shader->set("uf_proj_mat",  camera.get_proj_matrix());

    glBindVertexArray(sgrid.vao);
    glBindBuffer(GL_ARRAY_BUFFER, sgrid.vbo);

    glEnable(GL_BLEND);

    for (dv_scene_mesh& smesh : sgrid.meshes) {
        if (!smesh.ibo) continue;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smesh.ibo);
        glDrawElements(GL_TRIANGLES, (GLsizei)smesh.face_count * 3, GL_UNSIGNED_INT, 0);
    }

    glDisable(GL_BLEND);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

dv_shader* dv_sys_rendering::set_shader(dvsku::uuid id) {
    if (m_current_shader_id == id)
        return &m_systems->shader.shaders[m_current_shader_id];

    if (!m_systems->shader.shaders.contains(id))
        return nullptr;

    m_current_shader_id = id;
    m_systems->shader.shaders[m_current_shader_id].use();

    return &m_systems->shader.shaders[m_current_shader_id];
}