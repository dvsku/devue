#pragma once

#include "models/pdo/dv_mesh.hpp"
#include "rendering/dv_shader.hpp"
#include "scene/dv_camera.hpp"
#include "scene/dv_scene_grid.hpp"
#include "scene/lighting/dv_lighting.hpp"
#include "scene/model/dv_scene_material.hpp"
#include "scene/model/dv_scene_model.hpp"

#include <libutil.hpp>
#include <unordered_map>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_rendering {
    public:
    	dv_sys_rendering() = delete;
    	dv_sys_rendering(dv_systems_bundle* systems);

    	dv_sys_rendering(const dv_sys_rendering&) = delete;
    	dv_sys_rendering(dv_sys_rendering&&)	  = delete;

    	dv_sys_rendering& operator=(const dv_sys_rendering&) = delete;
    	dv_sys_rendering& operator=(dv_sys_rendering&&)		 = delete;

    public:
    	void prepare();
    	void prepare_model(dv_scene_model& smodel);
        void prepare_model(dv_scene_model& smodel, dv_model& model);
    	void prepare_model(dv_scene_mesh& smesh, dv_mesh& mesh, bool is_static = true);

    	void release_model(dv_scene_model& smodel);

    	void render(dv_scene_model& smodel, dv_camera& camera, dv_lighting& lighting);
    	void render(dv_scene_grid& sgrid, dv_camera& camera, dv_lighting& lighting);
    	
    private:
    	dv_systems_bundle* m_systems;
        dvsku::uuid        m_current_shader_id = 0U;

    private:
        // Set shader if it's not set already
        dv_shader* set_shader(dvsku::uuid id);
    };
}