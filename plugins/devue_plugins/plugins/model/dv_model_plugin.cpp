#include "dv_model_plugin.hpp"

#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <memory>

using namespace devue::plugins;

// Plugin instance
std::unique_ptr<dv_model_plugin> g_plugin = nullptr;

///////////////////////////////////////////////////////////////////////////////
// C API

DV_API dv_plugin_importer* create() {
    if (g_plugin) return g_plugin.get();

    try {
        g_plugin = std::make_unique<dv_model_plugin>();
        g_plugin->init();
    }
    catch (...) {
        return nullptr;
    }

    return g_plugin.get();
}

DV_API void release() {
    return g_plugin.reset();
}

DV_API uint8_t plugin_type() {
    return (uint8_t)dv_plugin_importer::plugin_type::model;
}

///////////////////////////////////////////////////////////////////////////////
// IMPL

void dv_model_plugin::init() {
    this->m_name    = "devue model plugin";
    this->m_author  = "";
    this->m_website = "";
    this->m_version = "1.0.0";
}

std::vector<dv_file_type> dv_model_plugin::_get_supported_types() {
    return {
        { "Wavefront object", ".obj"}
    };
}

dv_plugin_model dv_model_plugin::_import(const std::filesystem::path& filepath) {
    const aiScene* scene = importer.ReadFile(filepath.string().c_str(), aiProcess_Triangulate);

    if (!scene || !scene->mRootNode)
        return dv_plugin_model();

    dv_plugin_model model;

    for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[i];
        
        dv_plugin_mesh pmesh;
        pmesh.name = mesh->mName.C_Str();

        for (uint32_t j = 0; j < mesh->mNumVertices; j++) {
            const auto& pos    = mesh->mVertices[j];
            const auto& normal = mesh->mNormals[j];
            const auto& uv     = mesh->mTextureCoords[0][j];

            dv_plugin_vertex vertex{};
            vertex.position = { pos.x, pos.y, pos.z };
            vertex.normal   = { normal.x, normal.y, normal.z };
            vertex.uv       = { uv.x, uv.y };

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
