#pragma once

#include <functional>

namespace devue {
    class dv_util_command {
    public:
        using command_fn_t = std::function<bool()>;

    public:
        bool executable = false;

    public:
        dv_util_command() = default;
        dv_util_command(command_fn_t fn);

    private:
        command_fn_t m_fn = nullptr;
    };
}