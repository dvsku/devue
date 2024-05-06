#pragma once

#include "gui/base/dv_comp.hpp"

namespace devue {
    class dv_modal_import : public dv_comp {
    public:
        dv_modal_import() = delete;
        dv_modal_import(dv_systems* systems, dv_components* components);

    public:
        void set_file_path(const std::string& path);
        void set_materials_path(const std::string& path);

    public:
        dvsku::dv_command_state render();

    private:
        bool        m_keep_materials = false;
        std::string m_file_path      = "";
        std::string m_materials_path = "";

    private:
        void reset();
    };
}