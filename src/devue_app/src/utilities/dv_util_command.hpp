#pragma once

#include <functional>

namespace devue {
    #define DV_COMMAND_FINISHED false
    #define DV_COMMAND_REPEAT   true

    class dv_util_command {
    public:
        using command_fn_t = std::function<bool()>;

    public:
        bool executable = false;

    public:
        dv_util_command() = default;
        dv_util_command(command_fn_t fn);

    public:
        void execute();

    private:
        command_fn_t m_fn = nullptr;
    };
}