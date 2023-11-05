#pragma once

#include "plugins/dv_texture_plugin.hpp"
#include "utilities/dv_util_uuid.hpp"

#include <unordered_map>

namespace devue::core {
    class dv_sys_plugin {
    public:
        void prepare();

    private:
        std::unordered_map<devue::uuid, devue::core::dv_texture_plugin> m_texture_plugins;
    };
}