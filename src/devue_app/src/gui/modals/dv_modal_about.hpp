#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
    class dv_modal_about : public dv_comp {
    public:
        dv_modal_about() = delete;
        dv_modal_about(dv_systems* systems, dv_components* components);

    public:
        dvsku::dv_command_state render();
    };
}