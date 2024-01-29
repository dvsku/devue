#include "systems/dv_sys_texture.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "exceptions/dv_exception.hpp"
#include "utilities/dv_util_string.hpp"
#include "utilities/dv_util_log.hpp"
#include "glad/glad.h"

#include <filesystem>

using namespace devue::core;
using namespace devue::plugins;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_sys_texture::dv_sys_texture(dv_systems_bundle* systems) 
    : m_systems(systems) {}

const dv_scene_texture* dv_sys_texture::get_texture(devue::uuid uuid) {
    if (!m_textures.contains(uuid)) return nullptr;
    return &m_textures[uuid].second;
}

size_t dv_sys_texture::count() const {
    return m_textures.size();
}

void dv_sys_texture::prepare_material_textures(dv_model& model, 
                                               dv_material& material, dv_scene_material& smaterial) 
{
    if (!material.diffuse_texture.empty()) {
        std::filesystem::path texture = std::filesystem::path(model.texture_dir).append(material.diffuse_texture);
        devue::uuid uuid = dv_util_uuid::create(texture.string());
    
        try {
            if (m_textures.contains(uuid)) {
                m_textures[uuid].first++;
            }
            else {
                m_textures[uuid] = {
                    1U,
                    create_scene_texture(texture)
                };
            }
    
            smaterial.diffuse_texture_uuid = uuid;
        }
        catch (const std::exception& e) {
            DV_LOG("Failed to create `{}` diffuse texture `{}`. | {}", model.name, material.diffuse_texture, e.what());
        }
        catch (...) {
            DV_LOG("Failed to create `{}` diffuse texture `{}`.", model.name, material.diffuse_texture);
        }
    }
}

void dv_sys_texture::release_textures(dv_scene_material& smaterial) {
    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_textures.contains(smaterial.diffuse_texture_uuid)) {
        auto& [ref, texture] = m_textures[smaterial.diffuse_texture_uuid];

        if (ref > 1) {
            ref--;
        }
        else {
            glDeleteTextures(1, &texture.texture_id);
            m_textures.erase(smaterial.diffuse_texture_uuid);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

 dv_scene_texture dv_sys_texture::create_scene_texture(std::filesystem::path& filepath) {
    if (!std::filesystem::exists(filepath))
        throw dv_exception(DV_FORMAT("`{}` file not found.", filepath.string()));

    std::string ext = filepath.extension().string();

    auto cmp_fn = [&](const dv_file_type& type) {
        return dv_util_string::contains(type.extensions, ext);
    };

    for (auto& [uuid, plugin] : m_systems->plugin.plugins) {
        if (std::none_of(plugin.supported_texture_types.begin(), plugin.supported_texture_types.end(), cmp_fn))
            continue;

        devue_plugin_texture ptexture;

        try {
            ptexture = plugin.import_texture(filepath.string());
        }
        catch (...) {
            plugin.cleanup();
            continue;
        }

        plugin.cleanup();

        if (!ptexture.height || !ptexture.width || ptexture.data.empty()) 
            throw;

        dv_scene_texture stexture;

        glGenTextures(1, &stexture.texture_id);
        glBindTexture(GL_TEXTURE_2D, stexture.texture_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ptexture.width, ptexture.height, 
                     0, GL_RGB, GL_UNSIGNED_BYTE, ptexture.data.data());

        return stexture;
    }

    throw dv_exception("No suitable importer found.");
}
