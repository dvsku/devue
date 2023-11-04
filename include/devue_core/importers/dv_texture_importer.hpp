#pragma once

#include "importers/dv_file_type.hpp"
#include "plugins/dv_plugin_texture.hpp"

#include <string>
#include <vector>
#include <functional>

namespace devue::core {
    struct dv_texture_importer {
        typedef std::function<devue::shared::dv_plugin_texture(const std::string&)> import_fn;

        std::vector<dv_file_type> types;
        import_fn fn = nullptr;
    };
}