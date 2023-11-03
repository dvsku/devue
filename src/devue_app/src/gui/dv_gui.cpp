#include "gui/dv_gui.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "rendering/dv_multisample_frame_buffer.hpp"
#include "utilities/dv_util_log.hpp"

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

dv_gui::dv_gui(uint32_t width, uint32_t height, const std::string& title)
	: dv_opengl_window(width, height, title), m_components(&m_sytems) 
{
	m_scene_render_target = std::make_shared<dv_multisample_frame_buffer>(width, height);
}

dv_gui::~dv_gui() {}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

bool dv_gui::prepare() {
	// Create shaders
	try {
		m_sytems.rendering.prepare();
	}
	catch (const std::exception& e) {
		DV_LOG("Failed to prepare rendering system. | {}", e.what());
		return false;
	}

	// TODO: Load plugins here

	// Load model importers
	try {
		m_sytems.model.prepare();
	}
	catch (const std::exception& e) {
		DV_LOG("Failed to prepare model system. | {}", e.what());
		return false;
	}
	
	// Create a scene
	if (!m_sytems.scene.create_scene())
		return false;

	glfwMaximizeWindow(m_native);
	return true;
}

void dv_gui::on_before_update() {

}

void dv_gui::on_update() {
	m_sytems.scene.render_current_scene(m_scene_render_target.get());
}

void dv_gui::on_after_update() {}

void dv_gui::on_gui_before_update() {
	glClearColor(0.185f, 0.185f, 0.185f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void dv_gui::on_gui_update() {
	m_components.dockspace.render();
	m_components.hierarchy.render();
	m_components.assets.render();
	m_components.scene.render();
	m_components.properties.render();
	m_components.console.render();

	ImGui::ShowDemoWindow();
}

void dv_gui::on_gui_after_update() {}

void dv_gui::on_resize(int width, int height) {

}

void dv_gui::on_scroll(double dx, double dy) {

}

void dv_gui::on_mouse_button(int btn, int action, int modifier) {

}

void dv_gui::on_mouse_move(double dx, double dy) {

}
