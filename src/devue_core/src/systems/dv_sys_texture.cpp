#include "systems/dv_sys_texture.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "exceptions/dv_exception.hpp"
#include "utilities/dv_util_string.hpp"
#include "utilities/dv_util_log.hpp"
#include "glad/glad.h"

#include <filesystem>
#include <sstream>

using namespace devue::core;
using namespace devue::plugins;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_sys_texture::dv_sys_texture(dv_systems_bundle* systems) 
    : m_systems(systems) {}

const dv_scene_texture* dv_sys_texture::get_texture(devue::uuid uuid) {
    if (!textures.contains(uuid)) return nullptr;
    return &textures[uuid].second;
}

void dv_sys_texture::prepare_material_textures(dv_model& model, 
                                               dv_material& material, dv_scene_material& smaterial) 
{
    if (!material.diffuse_texture.empty()) {
        std::filesystem::path texture = std::filesystem::path(model.texture_dir).append(material.diffuse_texture);
        devue::uuid uuid = dv_util_uuid::create(texture.string());
    
        try {
            if (textures.contains(uuid)) {
                textures[uuid].first++;
            }
            else {
                textures[uuid] = {
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

    if (textures.contains(smaterial.diffuse_texture_uuid)) {
        auto& [ref, texture] = textures[smaterial.diffuse_texture_uuid];

        if (ref > 1) {
            ref--;
        }
        else {
            glDeleteTextures(1, &texture.texture_id);
            textures.erase(smaterial.diffuse_texture_uuid);
        }
    }
}

dv_scene_texture dv_sys_texture::create_scene_texture(plugins::devue_plugin_texture ptexture) {
    dv_scene_texture stexture;

    glGenTextures(1, &stexture.texture_id);
    glBindTexture(GL_TEXTURE_2D, stexture.texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    uint32_t format = 0U;
    switch (ptexture.components) {
        case 3: format = GL_RGB;  break;
        case 4: format = GL_RGBA; break;

        default: DV_THROW_EXCEPTION("Unsupported texture format.");
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ptexture.width, ptexture.height,
        0, format, GL_UNSIGNED_BYTE, ptexture.data.data());

    stexture.width = ptexture.width;
    stexture.height = ptexture.height;
    stexture.components = ptexture.components;

    return stexture;
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

dv_scene_texture dv_sys_texture::create_scene_texture(std::filesystem::path& filepath) {
    if (!std::filesystem::exists(filepath))
        DV_THROW_EXCEPTION("File not found.");

    std::string ext = filepath.extension().string();

    auto cmp_fn = [&](const dv_file_type& type) {
        return dv_util_string::contains(type.extensions, ext);
    };

    // Flag to check if we tried importing but all importers failed
    bool tried_importing = false;

    // Error messages for all tried importers that failed
    // Ignored if one of them succeeded
    std::stringstream accumulated_errors;

    for (auto& [uuid, plugin] : m_systems->plugin.plugins) {
        if (std::none_of(plugin.supported_texture_types.begin(), plugin.supported_texture_types.end(), cmp_fn))
            continue;

        // We have tried importing
        tried_importing = true;

        devue_plugin_texture ptexture;
        try {
            ptexture = plugin.import_texture(filepath.string());
        }
        catch (const std::exception& e) {
            accumulated_errors << DV_FORMAT("\t`{}`: {}", plugin.name, e.what());
            plugin.cleanup();
            continue;
        }
        catch (...) {
            accumulated_errors << DV_FORMAT("\t`{}`: critical failure", plugin.name);
            plugin.cleanup();
            continue;
        }

        plugin.cleanup();

        if (!ptexture.height || !ptexture.width || !ptexture.components || ptexture.data.empty()) {
            accumulated_errors << DV_FORMAT("\t`{}`: Incomplete texture data.", plugin.name);
            continue;
        }

        try {
            dv_scene_texture stexture = create_scene_texture(ptexture);
            return stexture;
        }
        catch (const std::exception& e) {
            accumulated_errors << DV_FORMAT("\t`{}`: {}", plugin.name, e.what());
            continue;
        }
        catch (...) {
            accumulated_errors << DV_FORMAT("\t`{}`: critical failure", plugin.name);
            continue;
        }
    }

    std::string errors = accumulated_errors.str();
    if (tried_importing && !errors.empty()) {
        DV_LOG("Tried importing texture with following importers:\n{}", errors);
    }

    DV_THROW_EXCEPTION("No suitable importer found.");
}

