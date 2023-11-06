#pragma once

#include "dv_plugin_model.hpp"
#include "dv_file_type.hpp"

#include <vector>
#include <string>
#include <functional>

namespace devue::core {
    struct dv_model_importer {
    	typedef std::function<devue::plugins::dv_plugin_model(const std::string&)> import_fn;

    	std::vector<devue::plugins::dv_file_type> types;
    	import_fn fn = nullptr;
    };
}