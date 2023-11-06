#pragma once

#include <cstdint>

namespace devue::plugins {
    class dv_plugin_importer {
    public:
        struct serialized {
            uint64_t size = 0U;
            uint8_t* data = nullptr;

            explicit operator bool() const {
                return size && data;
            }
        };

        ///////////////////////////////////////////////////////////////////////
        // PLUGIN DETAILS

        virtual serialized get_name()   = 0;
        virtual serialized get_author() = 0;
        virtual serialized get_link()   = 0;

        virtual uint8_t get_plugin_version_major() = 0;
        virtual float get_plugin_version_minor()   = 0;
        virtual uint32_t get_internal_version()    = 0;

        ///////////////////////////////////////////////////////////////////////
        // PLUGIN IMPL

        virtual void init()    = 0;
        virtual void cleanup() = 0;

        virtual serialized get_suported_types()         = 0;
        virtual serialized import(const char* filepath) = 0;
    };
}