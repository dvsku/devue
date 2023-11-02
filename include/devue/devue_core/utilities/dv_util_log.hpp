#pragma once

#include <format>
#include <string_view>
#include <iostream>
#include <vector>

namespace devue::core {
	#define DV_LOG(fmt, ...)	\
			devue::core::dv_util_log::log_message(fmt, ##__VA_ARGS__)

	class dv_util_log {
	public:
		struct source {
			bool enabled		 = true;
			std::ostream* stream = nullptr;
		};

	public:
		template <typename... Targs>
		static void log_message(const std::string_view& fmt, Targs&&... args) {
			if (!m_sources.size()) return;
			
			auto formatted = std::vformat(std::string(fmt) + "\n", std::make_format_args(std::forward<Targs>(args)...));
			
			for (auto& source : m_sources)
				if (source.stream && source.enabled)
					(*source.stream) << formatted;
		};

		static void add_source(std::ostream* stream) {
			m_sources.push_back({ true, stream });
		}

	private:
		inline static std::vector<source> m_sources;
	};
}