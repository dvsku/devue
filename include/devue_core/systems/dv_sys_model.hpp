#pragma once

#include "models/pdo/dv_model.hpp"
#include "importers/dv_file_filter.hpp"
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
        dv_model* get(const devue::uuid& uuid);
    	size_t count() const;

        const std::vector<dv_file_filter>& get_supported_file_types() const;
        void update_supported_file_types();

    	dv_model& import(const std::string& path, const std::string& texture_path = "");

    private:
        dv_systems_bundle* m_systems;

    	std::vector<dv_file_filter>    m_supported_file_types;
    };
}