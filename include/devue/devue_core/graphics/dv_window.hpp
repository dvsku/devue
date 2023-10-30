#pragma once

#include "graphics/dv_basic_window.hpp"

#include <string>

struct GLFWwindow;

namespace devue::core {
	class dv_window : public devue::common::dv_basic_window {
	public:
		dv_window(uint32_t width, uint32_t height, const std::string& title);
		~dv_window();

	public:
		void run() final override;

		GLFWwindow* get_native() {
			return m_native;
		}

	private:
		GLFWwindow* m_native	= nullptr;
		bool m_minimized		= false;

	private:
		void prepare()			final override;
		void on_update()		final override;
		void on_gui_update()	final override;

		void loop();
	};
}