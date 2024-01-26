#include "plugins/dv_plugin.hpp"
#include "exceptions/dv_exception.hpp"
#include "json/include/json.hpp"

using namespace devue::core;
using namespace devue::plugins;

void dv_plugin::prepare() {
    dv_plugin_importer::serialized serialized = m_importer->get_plugin_info();

    if (!serialized)
        throw dv_exception("");

    nlohmann::json json = nlohmann::json::from_bjdata(serialized.data, serialized.data + serialized.size);

    if (json.contains("name") && json["name"].is_string())
        name = json["name"];

    if (json.contains("author") && json["author"].is_string())
        author = json["author"];

    if (json.contains("website") && json["website"].is_string())
        website = json["website"];

    if (json.contains("version") && json["version"].is_string())
        version = json["version"];

    if (json.contains("supported_types") && json["supported_types"].is_array()) {
        for (auto& json_type : json["supported_types"]) {
            if (!json_type.contains("name")       || !json_type["name"].is_string())       continue;
            if (!json_type.contains("extensions") || !json_type["extensions"].is_string()) continue;

            dv_file_type file_type;
            file_type.name       = json_type["name"];
            file_type.extensions = json_type["extensions"];

            supported_file_types.push_back(file_type);
        }
    }

    if (name.empty())
        throw dv_exception("");
}
