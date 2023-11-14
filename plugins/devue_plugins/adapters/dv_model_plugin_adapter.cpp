#include "dv_model_plugin_adapter.hpp"
#include "dv_serialization.hpp"

using namespace devue::plugins;

dv_plugin_importer::serialized dv_model_plugin_adapter::import(const char* filepath) {
    dv_plugin_model model = _import(filepath);

    if (!model.meshes.size())
        return {};

    dv_bin_writer bw(m_buffer);

    bw << model.meshes.size();
    for (auto& mesh : model.meshes) {
        bw << mesh.name;
        bw << mesh.material.name << mesh.material.diffuse_texture;

        bw << mesh.vertices.size();
        for (auto& vertex : mesh.vertices) {
            bw << vertex.position.x << vertex.position.y << vertex.position.z;
            bw << vertex.normal.x   << vertex.normal.y   << vertex.normal.z;
            bw << vertex.uv.x       << vertex.uv.y;
        }

        bw << mesh.indices.size();
        for (auto& index : mesh.indices)
            bw << index;
    }

    return { m_buffer.size(), m_buffer.data() };
}
