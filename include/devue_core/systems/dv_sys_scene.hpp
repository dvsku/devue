#pragma once

#include "scene/dv_scene.hpp"
#include "models/pdo/dv_model.hpp"
#include "rendering/dv_render_target.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_scene {
    public:
    	dv_scene* current_scene = nullptr;

    public:
    	dv_sys_scene() = delete;
    	dv_sys_scene(dv_systems_bundle* systems);

    	dv_sys_scene(const dv_sys_scene&) = delete;
    	dv_sys_scene(dv_sys_scene&&)	  = delete;

    	dv_sys_scene& operator=(const dv_sys_scene&) = delete;
    	dv_sys_scene& operator=(dv_sys_scene&&)		 = delete;

    public:
        bool prepare();
        void release();

    	size_t count() const;

    	dv_scene* create_scene();

    	void render_current_scene(dv_render_target* render_target = nullptr);

    	void add_to_scene(dv_model& model);
    	void remove_from_scene(dv_scene_model& smodel);

    private:
    	dv_systems_bundle* m_systems = nullptr;
    	std::unordered_map<devue::uuid, dv_scene> m_scenes;

    private:
    	void remove_marked_models();
        void rescale_model(dv_model& model, dv_scene_model& smodel);
    };
}