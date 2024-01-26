#include "dv_model_plugin_adapter.hpp"
#include "lib/json.hpp"

using namespace devue::plugins;

dv_model_plugin_adapter::dv_model_plugin_adapter() {}

dv_plugin_importer::serialized dv_model_plugin_adapter::import(const char* filepath) noexcept {
    nlohmann::json json = nlohmann::json::object();

    try {
        dv_plugin_model model = _import(filepath);

        if (!model.meshes.size())
            return {};

        json["meshes"] = nlohmann::json::array();

        for (size_t i = 0; i < model.meshes.size(); i++) {
            json["meshes"][i] = nlohmann::json::object();
            
            auto& mesh      = model.meshes[i];
            auto& json_mesh = json["meshes"][i];

            json_mesh["name"] = mesh.name;

            json_mesh["material"]                    = nlohmann::json::object();
            json_mesh["material"]["name"]            = mesh.material.name;
            json_mesh["material"]["diffuse_texture"] = mesh.material.diffuse_texture;

            json_mesh["vertices"] = nlohmann::json::array();

            for (size_t j = 0; j < mesh.vertices.size(); j++) {
                json_mesh["vertices"][j] = nlohmann::json::object();

                auto& vertex      = mesh.vertices[j];
                auto& json_vertex = json_mesh["vertices"][j];

                json_vertex["position"]["x"] = vertex.position.x;
                json_vertex["position"]["y"] = vertex.position.y;
                json_vertex["position"]["z"] = vertex.position.z;

                json_vertex["normal"]["x"] = vertex.normal.x;
                json_vertex["normal"]["y"] = vertex.normal.y;
                json_vertex["normal"]["z"] = vertex.normal.z;

                json_vertex["uv"]["x"] = vertex.uv.x;
                json_vertex["uv"]["y"] = vertex.uv.y;
            }

            json_mesh["indices"] = nlohmann::json::array();

            for (size_t j = 0; j < mesh.indices.size(); j++) {
                json_mesh["indices"][j] = mesh.indices[j];
            }
        }

        m_buffer.clear();
        nlohmann::json::to_cbor(json, m_buffer);
    }
    catch (const std::exception& e) {
        return {};
    }
    catch (...) {
        return {};
    }

    return { m_buffer.size(), m_buffer.data() };
}
