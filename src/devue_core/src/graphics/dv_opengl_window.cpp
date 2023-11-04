#include "graphics/dv_opengl_window.hpp"

#include <windows.h>
#include <stdexcept>

#define GLFW_EXPOSE_NATIVE_WIN32

#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "utilities/dv_util_mouse.hpp"
#include "utilities/dv_util_log.hpp"

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

dv_opengl_window::dv_opengl_window(uint32_t width, uint32_t height, const std::string& title) {
    glfwSetErrorCallback(NULL);

    if (!glfwInit())
    	throw std::runtime_error("Failed to init glfw.");

    m_native = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (!m_native)
    	throw std::runtime_error("Failed to create window.");

    //ds_util_theme::update_title_bar_theme(native);

    glfwMakeContextCurrent(m_native);
    glfwSwapInterval(1);				// enable vsync

    glfwSetWindowUserPointer(m_native, this);

    glfwSetWindowSizeCallback(m_native, resize_callback);
    glfwSetScrollCallback(m_native, scroll_callback);
    glfwSetMouseButtonCallback(m_native, mouse_button_callback);
    glfwSetCursorPosCallback(m_native, mouse_move_callback);
    glfwSetWindowIconifyCallback(m_native, iconify_callback);

    // Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    	throw std::runtime_error("Failed to init glad.");
}

dv_opengl_window::~dv_opengl_window() {
    glfwDestroyWindow(m_native);
    glfwTerminate();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void dv_opengl_window::run() {
    // Init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_native, true);
    ImGui_ImplOpenGL3_Init();

    DV_LOG("OpenGL version: \t{}", (char*)glGetString(GL_VERSION));
    DV_LOG("OpenGL renderer: \t{}", (char*)glGetString(GL_RENDERER));
    DV_LOG("ImGUI version: \t\t{}", _get_imgui_ver());

    // Cancel running if prepare failed
    if (!prepare()) return;

    loop();
}

///////////////////////////////////////////////////////////////////////////////
// PROTECTED

bool dv_opengl_window::prepare() {
    return true;
}

void dv_opengl_window::on_before_update() {}

void dv_opengl_window::on_after_update() {}

void dv_opengl_window::on_gui_before_update() {}

void dv_opengl_window::on_gui_after_update() {}

void dv_opengl_window::on_resize(int width, int height) {}

void dv_opengl_window::on_scroll(double dx, double dy) {}

void dv_opengl_window::on_mouse_button(int btn, int action, int modifier) {}

void dv_opengl_window::on_mouse_move(double dx, double dy) {}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

void dv_opengl_window::loop() {
    while (!glfwWindowShouldClose(m_native)) {
    	if (m_minimized) {
    		glfwWaitEvents();
    		continue;
    	}

    	// OpenGL render pass
    	on_before_update();
    	on_update();
    	on_after_update();

    	// GUI render pass
    	ImGui_ImplOpenGL3_NewFrame();
    	ImGui_ImplGlfw_NewFrame();
    	ImGui::NewFrame();

    	on_gui_before_update();
    	on_gui_update();

    	ImGui::Render();
    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    	on_gui_after_update();

    	glfwSwapBuffers(m_native);
    	glfwPollEvents();
    }
}

void dv_opengl_window::resize_callback(GLFWwindow* window, int width, int height) {
    if (width == 0 || height == 0) return;

    glViewport(0, 0, width, height);

    dv_opengl_window* instance = static_cast<dv_opengl_window*>(glfwGetWindowUserPointer(window));
    instance->on_resize(width, height);
}

void dv_opengl_window::scroll_callback(GLFWwindow* window, double delta_x, double delta_y) {
    dv_opengl_window* instance = static_cast<dv_opengl_window*>(glfwGetWindowUserPointer(window));
    instance->on_scroll(delta_x, delta_y);
}

void dv_opengl_window::mouse_button_callback(GLFWwindow* window, int button, int action, int modifier) {
    dv_opengl_window* instance = static_cast<dv_opengl_window*>(glfwGetWindowUserPointer(window));
    instance->on_mouse_button(button, action, modifier);
}

void dv_opengl_window::mouse_move_callback(GLFWwindow* window, double x, double y) {
    dv_util_mouse::delta.x	  = x - dv_util_mouse::position.x;
    dv_util_mouse::delta.y	  = dv_util_mouse::position.y - y;
    dv_util_mouse::position.x = x;
    dv_util_mouse::position.y = y;

    dv_opengl_window* instance = static_cast<dv_opengl_window*>(glfwGetWindowUserPointer(window));
    instance->on_mouse_move(dv_util_mouse::delta.x, dv_util_mouse::delta.y);
}

void dv_opengl_window::iconify_callback(GLFWwindow* window, int iconified) {
    dv_opengl_window* instance = static_cast<dv_opengl_window*>(glfwGetWindowUserPointer(window));
    instance->m_minimized = (bool)iconified;
}
