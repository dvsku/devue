#pragma once

#include "gui/base/dv_comp.hpp"

#include <string>

namespace devue {
    class dv_comp_texture : public dv_comp {
    public:
        dv_comp_texture() = delete;
        dv_comp_texture(dv_systems* systems, dv_components* components);

    public:
        dvsku::gui_command_state render();
                           void set_texture(dvsku::uuid uuid, const std::string& name);

    private:
        dvsku::uuid m_texture_uuid = 0U;
        std::string m_texture_name = "";
    };
}