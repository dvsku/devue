#pragma once

#include <format>
#include <string_view>
#include <iostream>
#include <vector>
#include <chrono>

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
    		
            auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm local;
            localtime_s(&local, &time);
    
            auto formatted = std::vformat(fmt, std::make_format_args(std::forward<Targs>(args)...));         
            auto msg       = std::vformat("[{}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}] {}\n", 
                                          std::make_format_args(1900 + local.tm_year, local.tm_mon + 1, 
                                          local.tm_mday, local.tm_hour, local.tm_min, local.tm_sec, formatted));
    		
    		for (auto& source : m_sources)
    			if (source.stream && source.enabled)
    				(*source.stream) << msg;
    	};

    	static void add_source(std::ostream* stream) {
    		m_sources.push_back({ true, stream });
    	}

    private:
    	inline static std::vector<source> m_sources;
    };
}