#pragma once

#include <string>

namespace devue::core {
    struct dv_file_type {
    	std::string name		= "";
    	std::string extensions	= "";
    };

    struct dv_file_filter {
    	std::wstring name		= L"";
    	std::wstring extensions = L"";

    	dv_file_filter() = default;
    	dv_file_filter(std::wstring _name, std::wstring _ext) : name(_name), extensions(_ext) {}
    	dv_file_filter(const dv_file_type& file_type);
    };
}