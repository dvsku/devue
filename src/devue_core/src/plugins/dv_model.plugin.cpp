#include "plugins/dv_model_plugin.hpp"
#include "json/include/json.hpp"

using namespace devue::core;
using namespace devue::plugins;

dv_plugin_model dv_model_plugin::import(const std::string & filepath) {
    dv_plugin_model                model;
    dv_plugin_importer::serialized serialized;

    serialized = m_importer->import(filepath.c_str());

    if (!serialized)
        throw;

    nlohmann::json json = nlohmann::json::from_cbor(serialized.data, serialized.data + serialized.size);

    if (!json.contains("meshes") || json["meshes"].empty())
        return dv_plugin_model();

    for (auto& json_mesh : json["meshes"]) {
        dv_plugin_mesh mesh;

        if (json_mesh.contains("name") && json_mesh["name"].is_string())
            mesh.name = json_mesh["name"];

        if (json_mesh.contains("material") && json_mesh["material"].is_object()) {
            auto& json_material = json_mesh["material"];

            if (json_material.contains("name") && json_material["name"].is_string())
                mesh.material.name = json_material["name"];

            if (json_material.contains("diffuse_texture") && json_material["diffuse_texture"].is_string())
                mesh.material.diffuse_texture = json_material["diffuse_texture"];
        }

        if (json_mesh.contains("vertices") && json_mesh["vertices"].is_array()) {
            auto& json_vertices = json_mesh["vertices"];

            for (auto& json_vertex : json_vertices) {
                dv_plugin_vertex vertex{};

                if (json_vertex.contains("position")) {
                    auto& json_position = json_vertex["position"];

                    if (json_position.contains("x") && json_position["x"].is_number())
                        vertex.position.x = json_position["x"];

                    if (json_position.contains("y") && json_position["y"].is_number())
                        vertex.position.y = json_position["y"];

                    if (json_position.contains("z") && json_position["z"].is_number())
                        vertex.position.z = json_position["z"];
                }

                if (json_vertex.contains("normal")) {
                    auto& json_normal = json_vertex["normal"];

                    if (json_normal.contains("x") && json_normal["x"].is_number())
                        vertex.normal.x = json_normal["x"];

                    if (json_normal.contains("y") && json_normal["y"].is_number())
                        vertex.normal.y = json_normal["y"];

                    if (json_normal.contains("z") && json_normal["z"].is_number())
                        vertex.normal.z = json_normal["z"];
                }

                if (json_vertex.contains("uv")) {
                    auto& json_uv = json_vertex["uv"];

                    if (json_uv.contains("x") && json_uv["x"].is_number())
                        vertex.uv.x = json_uv["x"];

                    if (json_uv.contains("y") && json_uv["y"].is_number())
                        vertex.uv.y = json_uv["y"];
                }

                mesh.vertices.push_back(vertex);
            }

            if (json_mesh.contains("indices") && json_mesh["indices"].is_array()) {
                auto& json_indices = json_mesh["indices"];

                for (auto& json_index : json_indices) {
                    if (json_index.is_number())
                        mesh.indices.push_back(json_index);
                }
            }
        }

        model.meshes.push_back(mesh);
    }

    m_importer->cleanup();

    return model;
}