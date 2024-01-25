#include "gui/dv_gui.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "rendering/dv_multisample_frame_buffer.hpp"
#include "utilities/dv_util_diag.hpp"
#include "utilities/dv_util_log.hpp"

#include "gui/fonts/font_roboto.hpp"
#include "gui/fonts/font_fontawesome_solid.hpp"

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
    set_borderless();
    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;
    config.GlyphExtraSpacing.x  = 1.0f;
    config.RasterizerMultiply   = 1.2f;
    config.SizePixels           = 14.0f;

    // Add default font
    io.Fonts->AddFontFromMemoryTTF(ROBOTO_REGULAR, (int)ROBOTO_REGULAR_LEN, 14.0f, &config);

    // Add font awesome
    config.MergeMode                   = true;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromMemoryTTF(FONTAWESOME_SOLID, (int)FONTAWESOME_SOLID_LEN, 14.0f, &config, icon_ranges);

    // Build fonts
    io.Fonts->Build();

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

    // Load texture importers
    try {
        m_sytems.texture.prepare();
    }
    catch (const std::exception& e) {
        DV_LOG("Failed to prepare model system. | {}", e.what());
        return false;
    }

    try {
        m_sytems.plugin.prepare();
    }
    catch (const std::exception& e) {
        DV_LOG("Failed to prepare plugin system. | {}", e.what());
        return false;
    }
    
    if (!m_sytems.command.prepare(&m_components)) {
        DV_LOG("Failed to prepare command system.");
        return false;
    }
    
    m_sytems.command.set_execute(dv_commands::flag_show_console);

    // Create a scene
    auto scene = m_sytems.scene.create_scene();
    if (!scene)
    	return false;

    scene->lighting.ambient_light.intensity		= 0.2f;
    scene->lighting.directional_light.intensity = 0.55f;

    dv_util_diag::init();

    glfwMaximizeWindow(m_native);
    return true;
}

void dv_gui::on_before_update() {
    dv_util_diag::update();
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
    m_components.scene.render(m_scene_render_target.get());
    m_components.properties.render();

    m_sytems.command.execute();

    //ImGui::ShowDemoWindow();
}

void dv_gui::on_gui_after_update() {}

void dv_gui::on_resize(int width, int height) {
    if (m_scene_render_target)
    	m_scene_render_target->resize(width, height);

    if (m_sytems.scene.current_scene)
    	m_sytems.scene.current_scene->camera.set_aspect_ratio(static_cast<float>(width), static_cast<float>(height));
}

void dv_gui::on_scroll(double dx, double dy) {
    if (m_components.scene.is_hovered && m_sytems.scene.current_scene)
    	m_sytems.scene.current_scene->camera.zoom(static_cast<float>(dy));
}

void dv_gui::on_mouse_button(int btn, int action, int modifier) {

}

void dv_gui::on_mouse_move(double dx, double dy) {
    if (m_components.scene.is_hovered && m_sytems.scene.current_scene) {
    	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT)) {
    		m_sytems.scene.current_scene->camera.rotate(static_cast<float>(dx), static_cast<float>(dy));
    	}

    	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT)) {
    		m_sytems.scene.current_scene->camera.translate(static_cast<float>(dx), static_cast<float>(dy));
    	}
    }
}
