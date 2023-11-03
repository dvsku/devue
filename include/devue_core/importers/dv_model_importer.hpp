#pragma once

#include "models/pdo/dv_model.hpp"
#include "importers/dv_file_type.hpp"

#include <vector>
#include <functional>

namespace devue::core {
	struct dv_model_importer {
		typedef std::function<dv_model(const std::filesystem::path&)> import_fn;

		std::vector<dv_file_type> types;
		import_fn fn = nullptr;
	};
}