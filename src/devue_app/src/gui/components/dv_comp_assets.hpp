#pragma once

#include "gui/base/dv_comp.hpp"
#include "systems/dv_systems.hpp"

namespace devue {
    struct dv_comp_assets : public dv_comp {
    	dv_comp_assets(dv_systems* systems, dv_components* components);

    	bool render() final override;

    	void show_import_modal();

    private:
    	bool m_show_import_modal = false;

    	std::string m_import_file_path		= "";
    	std::string m_import_textures_path	= "";

    private:
    	void import_modal();
    };
}