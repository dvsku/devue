#pragma once

#include "models/pdo/dv_model.hpp"
#include "scene/model/dv_scene_material.hpp"
#include "scene/model/dv_scene_texture.hpp"
#include "devue_plugin_texture.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <utility>
#include <unordered_map>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_texture {
    public:
        dv_sys_texture() = delete;
        dv_sys_texture(dv_systems_bundle* systems);

        dv_sys_texture(const dv_sys_texture&) = delete;
        dv_sys_texture(dv_sys_texture&&)      = default;

        dv_sys_texture& operator=(const dv_sys_texture&) = delete;
        dv_sys_texture& operator=(dv_sys_texture&&)      = default;

    public:
        const dv_scene_texture* get_texture(devue::uuid uuid);
    	size_t count() const;

        void prepare_material_textures(dv_model& model, dv_material& material, dv_scene_material& smaterial);
        void release_textures(dv_scene_material& smaterial);

        dv_scene_texture create_scene_texture(plugins::devue_plugin_texture ptexture);

    private:
        dv_systems_bundle* m_systems;

    	std::unordered_map<devue::uuid, std::pair<uint16_t, dv_scene_texture>> m_textures;

    private:
        dv_scene_texture create_scene_texture(std::filesystem::path& filepath);
    };
}