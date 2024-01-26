#pragma once

#include <cstdint>

namespace devue::plugins {
    class dv_plugin_importer {
    public:
        struct serialized {
            uint64_t size = 0U;
            char*    data = nullptr;

            explicit operator bool() const {
                return size && data;
            }
        };

        enum class plugin_type : uint8_t {
            undefined = 0,
            model     = 1,
            texture   = 2
        };

    public:
        virtual serialized get_plugin_info()            noexcept = 0;
        virtual serialized import(const char* filepath) noexcept = 0;
        virtual void cleanup() noexcept = 0;
    };
}