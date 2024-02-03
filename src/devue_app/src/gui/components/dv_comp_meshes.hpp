#pragma once

#include "gui/base/dv_comp.hpp"
#include "models/pdo/dv_model.hpp"
#include "scene/model/dv_scene_model.hpp"

namespace devue {
    struct dv_comp_meshes : public dv_comp {
        dv_comp_meshes(dv_systems* systems, dv_components* components);

        bool render() final override;

    private:
        uuid m_current_mesh_id = 0U;

    private:
        void render_model();
        void render_scene_model();

        void render_mesh_list(core::dv_model& model);
        void render_mesh(core::dv_scene_mesh* smesh, core::dv_mesh& mesh, core::dv_material* material);
    };
}