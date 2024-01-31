#pragma once

#include "systems/dv_systems.hpp"

namespace devue {
    class dv_modal_plugins {
    public:
        dv_modal_plugins() = delete;
        dv_modal_plugins(dv_systems* systems);

    public:
        bool render();

    private:
        dv_systems* m_systems                 = nullptr;
        uuid m_current_plugin_id              = 0U;
        std::string m_supported_model_types   = "";
        std::string m_supported_texture_types = "";  
    };
}