#include "dv_texture_plugin_adapter.hpp"
#include "lib/json.hpp"

using namespace devue::plugins;

dv_texture_plugin_adapter::dv_texture_plugin_adapter() {}

dv_plugin_importer::serialized dv_texture_plugin_adapter::import(const char* filepath) noexcept {
    nlohmann::json json = nlohmann::json::object();

    try {
        dv_plugin_texture texture = _import(filepath);

        json["width"]  = texture.width;
        json["height"] = texture.height;
        json["data"]   = nlohmann::json::binary(std::move(texture.data));

        m_buffer.clear();
        nlohmann::json::to_cbor(json, m_buffer);
    }
    catch (const std::exception& e) {
        return {};
    }
    catch (...) {
        return {};
    }

    return { m_buffer.size(), m_buffer.data() };
}
