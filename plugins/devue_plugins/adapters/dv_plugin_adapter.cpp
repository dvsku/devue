#include "dv_plugin_adapter.hpp"
#include "lib/json.hpp"

using namespace devue::plugins;

dv_plugin_importer::serialized dv_plugin_adapter::get_plugin_info() noexcept {
    nlohmann::json json = nlohmann::json::object();

    try {
        json["name"]            = m_name;
        json["type"]            = (uint8_t)m_type;
        json["author"]          = m_author;
        json["website"]         = m_website;
        json["version"]         = m_version;
        json["supported_types"] = nlohmann::json::array();

        auto supported_types = _get_supported_types();
        for (size_t i = 0; i < supported_types.size(); i++) {
            auto& type = supported_types[i];

            json["supported_types"][i]               = nlohmann::json::object();
            json["supported_types"][i]["name"]       = type.name;
            json["supported_types"][i]["extensions"] = type.extensions;
        }

        m_buffer.clear();
        nlohmann::json::to_bjdata(json, m_buffer);
    }
    catch (const std::exception& e) {
        return {};
    }
    catch (...) {
        return {};
    }

    return { m_buffer.size(), m_buffer.data() };
}

void dv_plugin_adapter::cleanup() noexcept {
    m_buffer.clear();
}
