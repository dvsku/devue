#pragma once

#include "models/pdo/dv_model.hpp"

#include <dv_utilities.hpp>
#include <unordered_map>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_model {
    public:
    	std::unordered_map<dvsku::uuid, dv_model> models;

    public:
        dv_sys_model() = delete;
        dv_sys_model(dv_systems_bundle* systems);

        dv_sys_model(const dv_sys_model&) = delete;
        dv_sys_model(dv_sys_model&&)      = delete;

        dv_sys_model& operator=(const dv_sys_model&) = delete;
        dv_sys_model& operator=(dv_sys_model&&)      = delete;

    public:
        // Import model asset
    	bool import(const std::string& path, const std::string& material_path = "");

        // Mark model asset for removal
        void remove(dv_model& model);

        // Remove all model assets marked for removal
        void remove_marked_models();

    private:
        dv_systems_bundle* m_systems;

    };
}