#include "plugins/dv_model_plugin.hpp"
#include "dv_serialization.hpp"

using namespace devue::core;
using namespace devue::plugins;

dv_plugin_model dv_model_plugin::import(const std::string & filepath) {
    dv_plugin_model model;
    dv_plugin_importer::serialized serialized;

    serialized = m_importer->import(filepath.c_str());

    if (!serialized)
        throw;

    dv_bin_reader br(serialized.data, serialized.size);
    size_t meshes = 0U;

    br >> meshes;
    for (size_t i = 0; i < meshes; i++) {
        dv_plugin_mesh mesh;

        br >> mesh.name;
        br >> mesh.material.name >> mesh.material.diffuse_texture;

        size_t vertices = 0U;
        br >> vertices;
        for (size_t j = 0; j < vertices; j++) {
            dv_plugin_vertex vertex{};

            br >> vertex.position.x >> vertex.position.y >> vertex.position.z;
            br >> vertex.normal.x   >> vertex.normal.y   >> vertex.normal.z;
            br >> vertex.uv.x       >> vertex.uv.y;

            mesh.vertices.push_back(vertex);
        }

        size_t indices = 0U;
        br >> indices;
        for (size_t j = 0; j < indices; j++) {
            uint16_t index = 0U;
            br >> index;

            mesh.indices.push_back(index);
        }

        model.meshes.push_back(mesh);
    }

    m_importer->cleanup();

    return model;
}