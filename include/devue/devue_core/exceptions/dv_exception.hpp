#pragma once

#include <string>
#include <exception>

namespace devue::core {
	class dv_exception : public std::exception {
	public:
		dv_exception() {}
		dv_exception(const std::string& msg) : m_msg(msg) {}

		const char* what() const override {
			return m_msg.c_str();
		}

	private:
		std::string m_msg = "";
	};
}