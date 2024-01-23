#pragma once

#include "systems/dv_systems.hpp"

namespace devue {
    class dv_modal_import {
    public:
        dv_modal_import() = delete;
        dv_modal_import(dv_systems* systems);

    public:
        bool render();

    private:
        dv_systems* m_systems = nullptr;

        std::string m_file_path     = "";
        std::string m_textures_path = "";

    private:
        void reset();
    };
}