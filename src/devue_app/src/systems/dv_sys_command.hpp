#pragma once

#include "utilities/dv_util_command.hpp"

#include <map>

namespace devue {
    struct dv_systems;

    enum class dv_commands : uint16_t {
        flag_show_console
    };

    class dv_sys_command {
    public:
        std::map<dv_commands, dv_util_command> commands;

    public:
        dv_sys_command() = delete;
        dv_sys_command(dv_systems* systems);

        dv_sys_command(const dv_sys_command&) = delete;
        dv_sys_command(dv_sys_command&&)      = delete;

        dv_sys_command& operator=(const dv_sys_command&) = delete;
        dv_sys_command& operator=(dv_sys_command&&)      = delete;

    public:
        bool prepare();

        bool& is_executable(dv_commands command);
        void  set_execute(dv_commands command);

    private:
        dv_systems* m_systems = nullptr;
    };
}