#pragma once

#include <string>

struct GLFWwindow;

namespace devue::core {
    class dv_opengl_window {
    private:
        using handle_t   = void*;
        using longptr_t  = intptr_t;
        using ulongptr_t = size_t;

    public:
    	dv_opengl_window(uint32_t width, uint32_t height, const std::string& title);
    	virtual ~dv_opengl_window();

    	void run();

        static intptr_t wndproc_callback(dv_opengl_window* wnd, handle_t handle, uint32_t msg, ulongptr_t wparam, longptr_t lparam);

    protected:
    	GLFWwindow* m_native	= nullptr;
    	bool m_minimized		= false;

        // Move these to a separate struct

        int32_t m_custom_titlebar_height = 0;
        bool    m_skip_titlebar_hit      = false;
        bool    m_hover_maximize         = false;

    protected:
    	virtual bool prepare();

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
        virtual void on_drop(int count, const char* paths[]);

    protected:
        void set_borderless();

    private:
        intptr_t m_default_wndproc = 0;

    private:
    	void loop();

    	static void resize_callback(GLFWwindow* window, int width, int height);
    	static void scroll_callback(GLFWwindow* window, double dx, double dy);
    	static void mouse_button_callback(GLFWwindow* window, int button, int action, int modifier);
    	static void mouse_move_callback(GLFWwindow* window, double x, double y);
    	static void iconify_callback(GLFWwindow* window, int iconified);
        static void drop_callback(GLFWwindow* window, int count, const char* paths[]);
    };
}