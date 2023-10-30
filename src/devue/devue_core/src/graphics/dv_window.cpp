#include "graphics/dv_window.hpp"
#include "gui/dv_basic_gui.hpp"

#include <windows.h>
#include <stdexcept>

#define GLFW_EXPOSE_NATIVE_WIN32

#include "glad/glad.h"
#include "glfw/glfw3.h"

using namespace devue::core;
using namespace devue::common;

dv_window::dv_window(uint32_t width, uint32_t height, const std::string& title) {
	glfwSetErrorCallback(NULL);

	if (!glfwInit())
		throw std::runtime_error("Failed to init glfw.");

	m_native = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (!m_native)
		throw std::runtime_error("Failed to create window.");

	glfwMakeContextCurrent(m_native);
	glfwSwapInterval(1);				// enable vsync

	glfwSetWindowUserPointer(m_native, this);

	/*glfwSetWindowSizeCallback(native, resize_callback);
	glfwSetScrollCallback(native, scroll_callback);
	glfwSetMouseButtonCallback(native, mouse_button_callback);
	glfwSetCursorPosCallback(native, mouse_move_callback);
	glfwSetWindowIconifyCallback(native, iconify_callback);*/

	// Init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Failed to init glad.");
}

dv_window::~dv_window() {
	glfwDestroyWindow(m_native);
	glfwTerminate();
}

void dv_window::run() {
	prepare();
	loop();
}

void dv_window::prepare() {
	if (m_gui)
		m_gui->prepare();
}

void dv_window::on_update() {

}

void dv_window::on_gui_update() {
	if (m_gui)
		m_gui->update();
}

void dv_window::loop() {
	while (!glfwWindowShouldClose(m_native)) {
		if (m_minimized) {
			glfwWaitEvents();
			continue;
		}

		// OpenGL render pass
		//on_before_update();
		//on_update();
		//on_after_update();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		on_gui_update();

		glfwSwapBuffers(m_native);
		glfwPollEvents();
	}

	if (m_gui)
		m_gui->shutdown();
}
