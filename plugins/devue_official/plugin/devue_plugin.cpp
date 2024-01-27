#include "devue_plugin.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace devue::plugins;

///////////////////////////////////////////////////////////////////////////////
// IMPLEMENT

// Implement with your own details
void dv_model_plugin::impl_init() {
    this->name    = "official devue plugin";
    this->author  = "Nikola `dvsku` Stojsin";
    this->website = "https://github.com/dvsku/devue";
    this->version = "1.0.0";
}

// Implement your supported model types.
// Example model type:
//     { "Wavefront object", ".obj"}
// 
// If this is meant to be a texture plugin, return an empty vector.
std::vector<devue_plugin_base::file_type> dv_model_plugin::impl_get_model_types() {
    return {
        { "Wavefront object", ".obj"}
    };
}

// Implement your supported model types.
// Example model type:
//     { "JPEG", ".jpg;.jpeg"}
// 
// If this is meant to be a texture plugin, return an empty vector.
std::vector<devue_plugin_base::file_type> dv_model_plugin::impl_get_texture_types() {
    return {
        { "JPEG",   ".jpg;.jpeg"},
        { "PNG",    ".png"},
        { "Bitmap", ".bmp"},
    };
}

// Implement model importing
devue_plugin_model dv_model_plugin::impl_import_model(const std::filesystem::path& filepath) {
    const aiScene* scene = importer.ReadFile(filepath.string().c_str(), aiProcess_Triangulate);

    if (!scene || !scene->mRootNode)
        return devue_plugin_model();

    devue_plugin_model model;

    for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[i];

        devue_plugin_mesh pmesh;
        pmesh.name = mesh->mName.C_Str();

        for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
            const auto& pos = mesh->mVertices[j];
            const auto& normal = mesh->mNormals[j];
            const auto& uv = mesh->mTextureCoords[0][j];

            devue_plugin_vertex vertex{};
            vertex.position = { pos.x, pos.y, pos.z };
            vertex.normal = { normal.x, normal.y, normal.z };
            vertex.uv = { uv.x, uv.y };

            pmesh.vertices.push_back(vertex);
        }

        for (uint32_t j = 0; j < mesh->mNumFaces; j++) {
            const aiFace& face = mesh->mFaces[j];

            pmesh.indices.push_back(face.mIndices[0]);
            pmesh.indices.push_back(face.mIndices[1]);
            pmesh.indices.push_back(face.mIndices[2]);
        }

        if (scene->mNumMaterials - 1 >= mesh->mMaterialIndex) {
            auto& material = scene->mMaterials[mesh->mMaterialIndex];

            auto name = material->GetName();
            pmesh.material.name = name.C_Str();

            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                aiString diffuse_texture;
                material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuse_texture);
                pmesh.material.diffuse_texture = diffuse_texture.C_Str();
            }
        }

        model.meshes.push_back(pmesh);
    }

    return model;
}

// Implement texture importing
devue_plugin_texture dv_model_plugin::impl_import_texture(const std::filesystem::path& filepath) {
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

///////////////////////////////////////////////////////////////////////////////
// C API - DO NOT CHANGE

#include <memory>

std::unique_ptr<dv_model_plugin> plugin_instance = nullptr;

DV_API devue_plugin_interface* create() {
    if (plugin_instance) return plugin_instance.get();

    try {
        plugin_instance = std::make_unique<dv_model_plugin>();
        plugin_instance->impl_init();
    }
    catch (...) {
        return nullptr;
    }

    return plugin_instance.get();
}

DV_API void release() {
    return plugin_instance.reset();
}