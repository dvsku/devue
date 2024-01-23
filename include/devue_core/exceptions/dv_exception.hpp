#pragma once

#include <stdexcept>

namespace devue::core {
    class dv_exception : public std::runtime_error {
    public:
    	dv_exception() 
            : std::runtime_error("undefined") {}

    	dv_exception(const std::string& msg) 
            : std::runtime_error(msg) {}
    };
}