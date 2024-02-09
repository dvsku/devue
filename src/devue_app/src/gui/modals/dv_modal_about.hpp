#pragma once

#include "systems/dv_systems.hpp"

namespace devue {
    class dv_modal_about {
    public:
        dv_modal_about() = delete;
        dv_modal_about(dv_systems* systems);

    public:
        bool render();

    private:
        dv_systems* m_systems = nullptr;
    };
}