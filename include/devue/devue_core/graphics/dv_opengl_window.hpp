#pragma once

#include <string>

struct GLFWwindow;

namespace devue::core {
	class dv_opengl_window {
	public:
		dv_opengl_window(uint32_t width, uint32_t height, const std::string& title);
		virtual ~dv_opengl_window();

		void run();

	protected:
		GLFWwindow* m_native	= nullptr;
		bool m_minimized		= false;

	protected:
		virtual void prepare();

		virtual void on_before_update();
		virtual void on_update() = 0;
		virtual void on_after_update();

		virtual void on_gui_before_update();
		virtual void on_gui_update() = 0;
		virtual void on_gui_after_update();

		virtual void on_resize(int width, int height);
		virtual void on_scroll(double dx, double dy);
		virtual void on_mouse_button(int btn, int action, int modifier);
		virtual void on_mouse_move(double dx, double dy);

	private:
		void loop();

		static void resize_callback(GLFWwindow* window, int width, int height);
		static void scroll_callback(GLFWwindow* window, double dx, double dy);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int modifier);
		static void mouse_move_callback(GLFWwindow* window, double x, double y);
		static void iconify_callback(GLFWwindow* window, int iconified);
	};
}