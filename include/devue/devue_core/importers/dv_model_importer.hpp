#pragma once

#include "models/pdo/dv_model.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <vector>
#include <functional>

namespace devue::core {
	struct dv_model_importer {
		typedef std::function<dv_model(const std::filesystem::path&)> import_fn;

		std::vector<devue::uuid> types;
		import_fn fn = nullptr;
	};
}