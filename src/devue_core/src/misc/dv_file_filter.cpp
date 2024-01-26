#include "misc/dv_file_filter.hpp"
#include "utilities/dv_util_string.hpp"

#include <sstream>

using namespace devue::core;

dv_file_filter::dv_file_filter(const plugins::dv_file_type& file_type) {
    std::stringstream name_stream;

    name_stream << dv_util_string::trim(file_type.name, " ") << " (";
    std::string name_ext = dv_util_string::trim(file_type.extensions, " ");
    dv_util_string::replace(name_ext, ".", "*.");
    dv_util_string::replace(name_ext, ";", " ");
    name_stream << name_ext << ")";

    std::string ext = dv_util_string::trim(file_type.extensions, " ");
    dv_util_string::replace(ext, ".", "*.");

    name = dv_util_string::to_wide(name_stream.str());
    extensions = dv_util_string::to_wide(ext);
}