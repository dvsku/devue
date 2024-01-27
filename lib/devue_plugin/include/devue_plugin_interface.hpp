#pragma once

#include <cstdint>

namespace devue::plugins {
    class devue_plugin_interface {
    public:
        struct serialized {
            uint64_t size = 0U;
            char*    data = nullptr;

            explicit operator bool() const {
                return size && data;
            }
        };

    public:
        virtual serialized get_plugin_info() noexcept                    = 0;
        virtual serialized import_model(const char* filepath) noexcept   = 0;
        virtual serialized import_texture(const char* filepath) noexcept = 0;
        virtual void cleanup() noexcept                                  = 0;
    };
}