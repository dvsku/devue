#pragma once

#include "gui/base/dv_comp.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <string>

namespace devue {
    class dv_comp_texture : public dv_comp {
    public:
        dv_comp_texture(dv_systems* systems, dv_components* components);

    public:
        bool render() final override;

        void set_texture(uuid id, const std::string& name);

    private:
        uuid        m_texture_id   = 0U;
        std::string m_texture_name = "";
    };
}