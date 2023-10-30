#include "dv_window.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

using namespace devue;
using namespace devue::core;

static std::string _get_imgui_ver() {
	std::string str = std::string(ImGui::GetVersion());

#ifdef IMGUI_HAS_DOCK
	str += " docking";
#endif
#ifdef IMGUI_HAS_VIEWPORT
	str += " multi-viewport";
#endif

	return str;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_window::dv_window(uint32_t width, uint32_t height, const std::string& title)
	: dv_opengl_window(width, height, title) {}

dv_window::~dv_window() {}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

void dv_window::prepare() {


	glfwMaximizeWindow(m_native);
}

void dv_window::on_before_update() {

}

void dv_window::on_update() {

}

void dv_window::on_after_update() {}

void dv_window::on_gui_before_update() {
	glClearColor(0.185f, 0.185f, 0.185f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void dv_window::on_gui_update() {
	ImGui::ShowDemoWindow();
}

void dv_window::on_gui_after_update() {}

void dv_window::on_resize(int width, int height) {

}

void dv_window::on_scroll(double dx, double dy) {

}

void dv_window::on_mouse_button(int btn, int action, int modifier) {

}

void dv_window::on_mouse_move(double dx, double dy) {

}
