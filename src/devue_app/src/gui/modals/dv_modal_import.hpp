#pragma once

#include "systems/dv_systems.hpp"

namespace devue {
    class dv_modal_import {
    public:
        dv_modal_import() = delete;
        dv_modal_import(dv_systems* systems);

        void set_file_path(const std::string& path);
        void set_materials_path(const std::string& path);

    public:
        bool render();

    private:
        dv_systems* m_systems = nullptr;

        bool        m_keep_materials = false;
        std::string m_file_path      = "";
        std::string m_materials_path = "";

    private:
        void reset();
    };
}