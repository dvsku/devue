#include "devue_plugin_impl.hpp"
#include "devue_plugin.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "lib/tinyobjloader/tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

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

// Implement model importing
devue_plugin_model devue_plugin_impl::impl_import_model(const std::filesystem::path& filepath) {
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.triangulate = true;

    if (!reader.ParseFromFile(filepath.string(), reader_config)) {
        throw std::runtime_error("");
    }

    auto& attrib = reader.GetAttrib();

    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    devue_plugin_model model;
    model.meshes.push_back(devue_plugin_mesh());
    devue_plugin_mesh& mesh = model.meshes.back();

    std::unordered_map<devue_plugin_vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {
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
                //vertex.uv = {
                //    attrib.texcoords[2 * index.texcoord_index + 0],
                //    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                //};
            }

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(model.vertices.size());
                model.vertices.push_back(vertex);
            }

            mesh.indices.push_back(uniqueVertices[vertex]);

            /*mesh.vertices.push_back(vertex);
            mesh.indices.push_back(mesh.indices.size());*/
        }

        mesh.material.name = materials[shape.mesh.material_ids[0]].name;
        mesh.material.diffuse_texture = materials[shape.mesh.material_ids[0]].diffuse_texname;
    }
    
    return model;
}

// Implement texture importing
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
