#pragma once

#include "models/pdo/dv_model.hpp"
#include "scene/model/dv_scene_material.hpp"
#include "scene/model/dv_scene_texture.hpp"
#include "devue_plugin_texture.hpp"

#include <dv_gui_opengl/dv_gui_opengl.hpp>
#include <utility>
#include <unordered_map>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_texture {
    public:
        std::unordered_map<dvsku::uuid, std::pair<uint16_t, dv_scene_texture>> textures;

    public:
        dv_sys_texture() = delete;
        dv_sys_texture(dv_systems_bundle* systems);

        dv_sys_texture(const dv_sys_texture&) = delete;
        dv_sys_texture(dv_sys_texture&&)      = default;

        dv_sys_texture& operator=(const dv_sys_texture&) = delete;
        dv_sys_texture& operator=(dv_sys_texture&&)      = default;

    public:
        bool prepare();
        void release();

        const dv_scene_texture* get_texture(dvsku::uuid uuid);

        void prepare_material_textures(dv_model& model, dv_material& material, dv_scene_material& smaterial);
        void release_material_textures(dv_scene_material& smaterial);

        dv_scene_texture create_scene_texture(plugins::devue_plugin_texture ptexture);
        void release_texture(dv_scene_texture& texture);

    private:
        dv_systems_bundle* m_systems;

    private:
        dv_scene_texture create_scene_texture(std::filesystem::path& filepath);
    };
}