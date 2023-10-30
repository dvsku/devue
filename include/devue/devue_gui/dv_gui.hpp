#pragma once

#include "gui/dv_basic_gui.hpp"

struct GLFWwindow;

namespace devue::gui {
	class dv_gui : public devue::common::dv_basic_gui {
	public:
		dv_gui(GLFWwindow* native);

	public:
		void prepare()	final override;
		void update()	final override;
		void shutdown() final override;

	private:
		GLFWwindow* m_native = nullptr;

	private:
		void on_update();
	};
}