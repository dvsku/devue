#pragma once

#include "misc/dv_file_type.hpp"

namespace devue::core {
    struct dv_file_filter {
    	std::wstring name		= L"";
    	std::wstring extensions = L"";

    	dv_file_filter() = default;
        dv_file_filter(const dv_file_type& file_type);

    	dv_file_filter(std::wstring _name, std::wstring _ext) 
            : name(_name), extensions(_ext) {}    
    };
}