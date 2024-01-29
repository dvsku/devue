#include "devue_plugin_base.hpp"
#include "json.hpp"

using namespace devue::plugins;

devue_plugin_interface::serialized devue_plugin_base::get_plugin_info() noexcept {
    nlohmann::json json = nlohmann::json::object();

    try {
        json["name"]          = name;
        json["author"]        = author;
        json["website"]       = website;
        json["version"]       = version;
        json["model_types"]   = nlohmann::json::array();
        json["texture_types"] = nlohmann::json::array();

        auto model_types = impl_get_model_types();
        for (size_t i = 0; i < model_types.size(); i++) {
            auto& type = model_types[i];

            json["model_types"][i]               = nlohmann::json::object();
            json["model_types"][i]["name"]       = type.name;
            json["model_types"][i]["extensions"] = type.extensions;
        }

        auto texture_types = impl_get_texture_types();
        for (size_t i = 0; i < texture_types.size(); i++) {
            auto& type = texture_types[i];

            json["texture_types"][i]               = nlohmann::json::object();
            json["texture_types"][i]["name"]       = type.name;
            json["texture_types"][i]["extensions"] = type.extensions;
        }

        m_buffer.clear();
        nlohmann::json::to_bjdata(json, m_buffer);
    }
    catch (...) {
        return {};
    }

    return { m_buffer.size(), m_buffer.data() };
}

devue_plugin_interface::serialized devue_plugin_base::import_model(const char* filepath) noexcept {
    nlohmann::json json = nlohmann::json::object();

    try {
        devue_plugin_model model = impl_import_model(filepath);

        if (!model.meshes.size())
            return {};

        json["vertices"] = nlohmann::json::array();
        json["meshes"]   = nlohmann::json::array();

        for (size_t j = 0; j < model.vertices.size(); j++) {
            json["vertices"][j] = nlohmann::json::object();

            auto& vertex      = model.vertices[j];
            auto& json_vertex = json["vertices"][j];

            json_vertex["position"]["x"] = vertex.position.x;
            json_vertex["position"]["y"] = vertex.position.y;
            json_vertex["position"]["z"] = vertex.position.z;

            json_vertex["normal"]["x"] = vertex.normal.x;
            json_vertex["normal"]["y"] = vertex.normal.y;
            json_vertex["normal"]["z"] = vertex.normal.z;

            json_vertex["uv"]["x"] = vertex.uv.x;
            json_vertex["uv"]["y"] = vertex.uv.y;
        }

        for (size_t i = 0; i < model.meshes.size(); i++) {
            json["meshes"][i] = nlohmann::json::object();

            auto& mesh      = model.meshes[i];
            auto& json_mesh = json["meshes"][i];

            json_mesh["name"] = mesh.name;

            json_mesh["material"] = nlohmann::json::object();
            json_mesh["material"]["name"]            = mesh.material.name;
            json_mesh["material"]["diffuse_texture"] = mesh.material.diffuse_texture;

            json_mesh["indices"] = nlohmann::json::array();

            for (size_t j = 0; j < mesh.indices.size(); j++) {
                json_mesh["indices"][j] = mesh.indices[j];
            }
        }

        m_buffer.clear();
        nlohmann::json::to_cbor(json, m_buffer);
    }
    catch (...) {
        return {};
    }

    return { m_buffer.size(), m_buffer.data() };
}

devue_plugin_interface::serialized devue_plugin_base::import_texture(const char* filepath) noexcept {
    nlohmann::json json = nlohmann::json::object();

    try {
        devue_plugin_texture texture = impl_import_texture(filepath);

        json["width"]  = texture.width;
        json["height"] = texture.height;
        json["data"]   = nlohmann::json::binary(std::move(texture.data));

        m_buffer.clear();
        nlohmann::json::to_cbor(json, m_buffer);
    }
    catch (...) {
        return {};
    }

    return { m_buffer.size(), m_buffer.data() };
}

void devue_plugin_base::cleanup() noexcept {
    m_buffer.clear();
}
