#pragma once

#include "misc/dv_file_filter.hpp"
#include "models/pdo/dv_model.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
    struct dv_systems_bundle;

    class dv_sys_model {
    public:
    	std::unordered_map<devue::uuid, dv_model> models;

    public:
        dv_sys_model() = delete;
        dv_sys_model(dv_systems_bundle* systems);

        dv_sys_model(const dv_sys_model&) = delete;
        dv_sys_model(dv_sys_model&&)      = delete;

        dv_sys_model& operator=(const dv_sys_model&) = delete;
        dv_sys_model& operator=(dv_sys_model&&)      = delete;

    public:
        // Get windows open dialog file filters for all supported model formats
        const std::vector<dv_file_filter>& get_supported_file_types() const;

        // Update windows open dialog file filters for all supported model formats
        // Supported file formats are pulled from plugins
        void update_supported_file_types();

        // Import model asset
    	dv_model& import(const std::string& path, const std::string& texture_path = "");

        // Mark model asset for removal
        void remove(dv_model& model);

        // Remove all model assets marked for removal
        void remove_marked_models();

    private:
        dv_systems_bundle*          m_systems;
    	std::vector<dv_file_filter> m_supported_file_types;
    };
}