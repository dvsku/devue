#pragma once

#include "systems/dv_systems_bundle.hpp"
#include "systems/dv_sys_command.hpp"

namespace devue {
    struct dv_systems : core::dv_systems_bundle {
        dv_sys_command command;

    	dv_systems();
    };
}