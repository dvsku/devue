#include "utilities/dv_util_command.hpp"

using namespace devue;

dv_util_command::dv_util_command(command_fn_t fn) {
    m_fn = fn;
}
