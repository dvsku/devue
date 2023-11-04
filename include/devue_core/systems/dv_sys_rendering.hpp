#pragma once

#include "models/pdo/dv_mesh.hpp"
#include "scene/model/dv_scene_model.hpp"
#include "scene/model/dv_scene_material.hpp"
#include "scene/lighting/dv_lighting.hpp"
#include "scene/dv_scene_grid.hpp"
#include "scene/dv_camera.hpp"
#include "rendering/dv_shader.hpp"

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

    	void prepare();
    	void prepare_model(dv_scene_model& smodel);
    	void prepare_model(dv_scene_mesh& smesh, dv_mesh& mesh, bool is_static = true);

    	void release_model(dv_scene_model& smodel);

    	void render(dv_scene_model& smodel, dv_camera& camera, dv_lighting& lighting);
    	void render(dv_scene_grid& sgrid, dv_camera& camera, dv_lighting& lighting);
    	
    private:
    	dv_systems_bundle* m_systems;
    	std::unordered_map<uint8_t, dv_shader> m_shaders;

    private:
    	std::string get_shader_source(const std::string& path);
    	dv_shader create_shader(const std::string& vertex, const std::string& fragment);

    	void render_solid(dv_scene_model& smodel, dv_scene_mesh& smesh,
    					  dv_camera& camera, dv_lighting& lighting, glm::mat4& mvp, glm::mat3& nm);

    	void render_textured(const dv_scene_material* smaterial, dv_scene_mesh& smesh,
    						  dv_camera& camera, dv_lighting& lighting, glm::mat4& mvp, glm::mat3& nm);

    	void render_grid(dv_scene_model& smodel, dv_scene_mesh& smesh, dv_camera& camera);
    };
}