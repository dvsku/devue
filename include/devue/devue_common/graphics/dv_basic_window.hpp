#pragma once

namespace devue::common {
	class dv_basic_gui;

	class dv_basic_window {
	public:
		dv_basic_window()						= default;
		dv_basic_window(const dv_basic_window&) = delete;
		dv_basic_window(dv_basic_window&&)		= delete;

		dv_basic_window& operator=(const dv_basic_window&)	= delete;
		dv_basic_window& operator=(dv_basic_window&&)		= delete;

	public:
		virtual void run() = 0;

		void attach_gui(dv_basic_gui& gui) {
			m_gui = &gui;
		};

	protected:
		dv_basic_gui* m_gui = nullptr;

	protected:
		virtual void prepare() = 0;

		virtual void on_update()		= 0;
		virtual void on_gui_update()	= 0;
	};
}
