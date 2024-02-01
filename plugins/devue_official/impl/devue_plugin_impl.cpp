#include "devue_plugin_impl.hpp"
#include "devue_plugin.hpp"
#include "glm/gtx/normal.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/include/tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <algorithm>

using namespace devue::plugins;

void devue_plugin_impl::impl_init(devue_plugin& plugin) {
    plugin.name    = "devue official plugin";
    plugin.author  = "Nikola `dvsku` Stojsin";
    plugin.website = "https://github.com/dvsku/devue";
    plugin.version = "1.0.0";
}

std::vector<file_type> devue_plugin_impl::impl_get_model_types() {
    return {
        { "Wavefront object", ".obj"}
    };
}

std::vector<file_type> devue_plugin_impl::impl_get_texture_types() {
    return {
        { "JPEG",   ".jpg;.jpeg"},
        { "PNG",    ".png"},
        { "Bitmap", ".bmp"},
    };
}

devue_plugin_model devue_plugin_impl::impl_import_model(const std::filesystem::path& filepath) {
    tinyobj::ObjReader       reader;
    tinyobj::ObjReaderConfig reader_config;

    reader_config.vertex_color = false;

    // Triangulate has problems, disable it and require
    // that models are triangle only
    reader_config.triangulate = false;

    if (!reader.ParseFromFile(filepath.string(), reader_config))
        throw std::runtime_error(reader.Error());

    auto& attrib    = reader.GetAttrib();
    auto& shapes    = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    for (const auto& shape : shapes) {
        for (const auto& vert_per_face : shape.mesh.num_face_vertices) {
            if (vert_per_face != 3) {
                throw std::runtime_error("Only triangle faces supported.");
            }
        }
    }

    devue_plugin_model                                model;
    std::unordered_map<devue_plugin_vertex, uint32_t> unique_vertices{};

    for (const auto& material : materials) {
        model.materials.push_back(devue_plugin_material());
        model.materials.back().name            = material.name;
        model.materials.back().diffuse_texture = material.diffuse_texname;
    }

    for (const auto& shape : shapes) {
        model.meshes.push_back(devue_plugin_mesh());

        devue_plugin_mesh& mesh = model.meshes.back();
        mesh.name               = shape.name;
        mesh.material_index     = shape.mesh.material_ids[0];

        for (const auto& index : shape.mesh.indices) {
            devue_plugin_vertex vertex;

            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            if (index.normal_index >= 0) {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };
            }

            if (index.texcoord_index >= 0) {
                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            // Handle duplicate vertices

            if (!unique_vertices.contains(vertex)) {
                unique_vertices[vertex] = static_cast<uint32_t>(model.vertices.size());
                model.vertices.push_back(vertex);
            }

            mesh.indices.push_back(unique_vertices[vertex]);
        }
    }

    bool calculate_normals = std::all_of(model.vertices.begin(), model.vertices.end(), [](const devue_plugin_vertex& vertex) {
        return vertex.normal.x == 0.0f && vertex.normal.y == 0.0f && vertex.normal.z == 0.0f;
    });

    if (calculate_normals) {
        for (const auto& mesh : model.meshes) {
            if ((mesh.indices.size() % 3) != 0) continue;

            for (size_t i = 0; i < mesh.indices.size(); i += 3) {
                devue_plugin_vertex& v1 = model.vertices[mesh.indices[i + 0]];
                devue_plugin_vertex& v2 = model.vertices[mesh.indices[i + 1]];
                devue_plugin_vertex& v3 = model.vertices[mesh.indices[i + 2]];

                glm::vec3 normal = glm::triangleNormal(v1.position, v2.position, v3.position);

                v1.normal += normal;
                v2.normal += normal;
                v3.normal += normal;
            }
        }

        for (auto& vertex : model.vertices) {
            vertex.normal = glm::normalize(vertex.normal);
        }
    }

    return model;
}

devue_plugin_texture devue_plugin_impl::impl_import_texture(const std::filesystem::path& filepath) {
    devue_plugin_texture texture;

    stbi_set_flip_vertically_on_load(true);

    int channels = 0;
    unsigned char* texture_data =
        stbi_load(filepath.string().c_str(), &texture.width, &texture.height, &channels, 0);

    if (!texture_data) {
        stbi_image_free(texture_data);
        return devue_plugin_texture();
    }

    size_t size = (size_t)texture.width * texture.height * channels;
    texture.data.insert(texture.data.begin(), texture_data, texture_data + size);

    stbi_image_free(texture_data);

    return texture;
}
