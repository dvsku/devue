#pragma once

#include <windows.h>
#include <format>
#include <vector>

namespace devue::core {
    #define DV_FORMAT(fmt, ...)		\
    		std::vformat(fmt, std::make_format_args(__VA_ARGS__))

    #define DV_FORMAT_C(fmt, ...)	\
    		std::vformat(fmt, std::make_format_args(__VA_ARGS__)).c_str()
    
    class dv_util_string {
    public:
    	static std::wstring to_wide(const std::string& str) {
    		int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), NULL, 0);
    		std::wstring wstr(count, 0);
    		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.length(), &wstr[0], count);
    		return wstr;
    	}

    	static std::vector<std::string> split(const std::string str, const char seperator) {
    		std::vector<std::string> results;

    		std::string::size_type prev_pos = 0, pos = 0;

    		while ((pos = str.find(seperator, pos)) != std::string::npos) {
    			results.push_back(str.substr(prev_pos, pos - prev_pos));
    			prev_pos = ++pos;
    		}

    		results.push_back(str.substr(prev_pos, pos - prev_pos));

    		return results;
    	}

    	static bool contains(const std::string& haystack, const std::string& needle) {
    		return haystack.find(needle) != std::string::npos;
    	}

    	static std::string trim(const std::string& str, const std::string& character_to_trim) {
    		if (str.empty()) return str;

    		size_t start = 0;
    		while (start < str.size() && character_to_trim.find(str[start]) != std::string::npos) {
    			start++;
    		}

    		size_t end = str.size();
    		while (end > start && character_to_trim.find(str[end - 1]) != std::string::npos) {
    			end--;
    		}

    		return str.substr(start, end - start);
    	}

        static std::string replace(const std::string& source, const std::string& what, const std::string& with) {
            std::string replaced = source;
            
            size_t startPos = 0;
            while ((startPos = replaced.find(what, startPos)) != std::string::npos) {
                replaced.replace(startPos, what.length(), with);
                startPos += with.length();
            }

            return replaced;
        }

    	static void replace(std::string& source, const std::string& what, const std::string& with) {
    		size_t startPos = 0;
    		while ((startPos = source.find(what, startPos)) != std::string::npos) {
    			source.replace(startPos, what.length(), with);
    			startPos += with.length();
    		}
    	}
    };
}