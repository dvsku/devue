#pragma once

#include <filesystem>
#include <unordered_map>
#include <functional>

#include "models/pdo/dv_model.hpp"
#include "importers/dv_model_importer.hpp"
#include "utilities/dv_util_uuid.hpp"

namespace devue::core {
	class dv_sys_model {
	public:
		typedef std::filesystem::path FILE_PATH;

		std::unordered_map<devue::uuid, dv_model> models;

	public:
		void prepare();

		// Get a model
		dv_model* get(const devue::uuid& uuid);

		// Get import filters
		const std::vector<dv_file_filter>& get_import_filters();

		// Import a model
		dv_model& import(const FILE_PATH& path);

	private:
		std::vector<dv_model_importer> m_importers;
		std::vector<dv_file_filter> m_import_filters;

	private:
		void create_filters();
	};
}