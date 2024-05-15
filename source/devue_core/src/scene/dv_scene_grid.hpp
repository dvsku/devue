#pragma once

#include "models/pdo/dv_model.hpp"
#include "scene/model/dv_scene_model.hpp"

namespace devue::core {
    class dv_scene_grid : public dv_scene_model {
    public:
    	dv_model model;

    public:
    	dv_scene_grid();
    };
}
