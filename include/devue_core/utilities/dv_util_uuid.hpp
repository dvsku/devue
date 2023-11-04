#pragma once

#include <cstdint>
#include <string>

namespace devue {
    typedef uint64_t uuid;
}

namespace devue::core {
    struct dv_util_uuid {
        static uuid create();
        static uuid create(const std::string& str);
    };
}
