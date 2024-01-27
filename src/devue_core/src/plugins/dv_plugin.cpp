#include "plugins/dv_plugin.hpp"
#include "exceptions/dv_exception.hpp"
#include "json/include/json.hpp"

using namespace devue;
using namespace devue::core;

void dv_plugin::prepare() {
    plugins::devue_plugin_interface::serialized serialized = m_iface->get_plugin_info();

    if (!serialized)
        throw dv_exception("");

    nlohmann::json json = nlohmann::json::from_bjdata(serialized.data, serialized.data + serialized.size);

    if (json.contains("name") && json["name"].is_string())
        name = json["name"];

    if (json.contains("author") && json["author"].is_string())
        author = json["author"];

    if (json.contains("website") && json["website"].is_string())
        website = json["website"];

    if (json.contains("version") && json["version"].is_string())
        version = json["version"];

    if (json.contains("model_types") && json["model_types"].is_array()) {
        for (auto& json_type : json["model_types"]) {
            if (!json_type.contains("name")       || !json_type["name"].is_string())       continue;
            if (!json_type.contains("extensions") || !json_type["extensions"].is_string()) continue;

            dv_file_type file_type;
            file_type.name       = json_type["name"];
            file_type.extensions = json_type["extensions"];

            supported_model_types.push_back(file_type);
        }
    }

    if (json.contains("texture_types") && json["texture_types"].is_array()) {
        for (auto& json_type : json["texture_types"]) {
            if (!json_type.contains("name")       || !json_type["name"].is_string())       continue;
            if (!json_type.contains("extensions") || !json_type["extensions"].is_string()) continue;

            dv_file_type file_type;
            file_type.name       = json_type["name"];
            file_type.extensions = json_type["extensions"];

            supported_texture_types.push_back(file_type);
        }
    }

    if (name.empty())
        throw dv_exception("");
}

plugins::devue_plugin_model dv_plugin::import_model(const std::string& filepath) {
    plugins::devue_plugin_model                 model;
    plugins::devue_plugin_interface::serialized serialized;

    serialized = m_iface->import_model(filepath.c_str());

    if (!serialized)
        throw;

    nlohmann::json json = nlohmann::json::from_cbor(serialized.data, serialized.data + serialized.size);

    if (!json.contains("meshes") || json["meshes"].empty())
        return plugins::devue_plugin_model();

    for (auto& json_mesh : json["meshes"]) {
        plugins::devue_plugin_mesh mesh;

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
                plugins::devue_plugin_vertex vertex{};

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

    return model;
}

plugins::devue_plugin_texture dv_plugin::import_texture(const std::string& filepath) {
    plugins::devue_plugin_texture               texture;
    plugins::devue_plugin_interface::serialized serialized;

    serialized = m_iface->import_texture(filepath.c_str());

    if (!serialized)
        throw;

    nlohmann::json json = nlohmann::json::from_cbor(serialized.data, serialized.data + serialized.size);

    if (json.contains("width") && json["width"].is_number())
        texture.width = json["width"];

    if (json.contains("height") && json["height"].is_number())
        texture.height = json["height"];

    if (json.contains("data") && json["data"].is_binary())
        texture.data = std::move(json["data"].get_binary());

    return texture;
}

void dv_plugin::cleanup() {
    m_iface->cleanup();
}
