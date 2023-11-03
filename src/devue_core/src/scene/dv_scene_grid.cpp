#include "scene/dv_scene_grid.hpp"
#include "utilities/dv_util_uuid.hpp"

using namespace devue::core;

dv_scene_grid::dv_scene_grid() {
    dv_mesh mesh;

    dv_vertex v1, v2, v3, v4;
    v1.position = glm::vec3(-1.0f, -1.0f, 0.0f);
    v2.position = glm::vec3(1.0f, -1.0f, 0.0f);
    v3.position = glm::vec3(1.0f, 1.0f, 0.0f);
    v4.position = glm::vec3(-1.0f, 1.0f, 0.0f);

    mesh.vertices.push_back(v1);
    mesh.vertices.push_back(v2);
    mesh.vertices.push_back(v3);
    mesh.vertices.push_back(v4);

    mesh.faces.push_back({ 0, 1, 2 });
    mesh.faces.push_back({ 2, 3, 0 });

    model.meshes[dv_util_uuid::create("grid")] = mesh;

    dv_scene_mesh rmesh;
    meshes.push_back(rmesh);
}