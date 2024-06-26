#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
    class dv_modal_plugins : public dv_comp {
    public:
        dv_modal_plugins() = delete;
        dv_modal_plugins(dv_systems* systems, dv_components* components);

    public:
        libgui::command::state render();

    private:
        libutil::uuid m_current_plugin_id       = 0U;
        std::string   m_supported_model_types   = "";
        std::string   m_supported_texture_types = "";
    };
}