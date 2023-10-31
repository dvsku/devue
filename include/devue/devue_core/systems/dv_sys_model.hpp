#pragma once

#include <filesystem>
#include <unordered_map>

#include "utilities/dv_util_uuid.hpp"

namespace devue::core {
	class dv_sys_model {
	public:
		typedef std::filesystem::path FILE_PATH;

	public:
		std::unordered_map<devue::uuid, int> models;

	public:
		int* get(const devue::uuid& uuid);
		int& import(const FILE_PATH& path);
	};
}