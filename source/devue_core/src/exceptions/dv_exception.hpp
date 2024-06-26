#pragma once

#include <libutil.hpp>
#include <stdexcept>

namespace devue::core {
    #define DV_THROW_EXCEPTION(fmt, ...)    \
        throw devue::core::dv_exception(libutil::string::format(fmt, __VA_ARGS__))

    class dv_exception : public std::runtime_error {
    public:
    	dv_exception() 
            : std::runtime_error("undefined") {}

    	dv_exception(const std::string& msg) 
            : std::runtime_error(msg) {}
    };
}