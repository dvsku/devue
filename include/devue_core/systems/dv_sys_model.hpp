#pragma once

#include "models/pdo/dv_model.hpp"
#include "importers/dv_model_importer.hpp"
#include "importers/dv_file_filter.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
    class dv_sys_model {
    public:
    	std::unordered_map<devue::uuid, dv_model> models;

    public:
        dv_model* get(const devue::uuid& uuid);
    	size_t count() const;

        const std::vector<dv_file_filter>& get_supported_file_types() const;
        void update_supported_file_types();

        void create_importer(dv_model_importer&& importer);
        void release_importers();

    	dv_model& import(const std::string& path, const std::string& texture_path = "");

    private:
    	std::vector<dv_model_importer> m_importers;
    	std::vector<dv_file_filter>    m_supported_file_types;
    };
}