#pragma once

#include <format>

namespace devue::core {
	#define DV_FORMAT(fmt, ...)		\
			std::vformat(fmt, std::make_format_args(__VA_ARGS__))

	#define DV_FORMAT_C(fmt, ...)	\
			std::vformat(fmt, std::make_format_args(__VA_ARGS__)).c_str()
	
	class dv_util_string {

	};
}