#pragma once

#include "model/dv_plugin_model.hpp"
#include "importers/dv_file_type.hpp"

#include <vector>
#include <string>
#include <functional>

namespace devue::core {
    struct dv_model_importer {
    	typedef std::function<devue::shared::dv_plugin_model(const std::string&)> import_fn;

    	std::vector<dv_file_type> types;
    	import_fn fn = nullptr;
    };
}