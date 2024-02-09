#include "gui/dv_gui.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "rendering/dv_multisample_frame_buffer.hpp"
#include "utilities/dv_util_diag.hpp"
#include "utilities/dv_util_log.hpp"
#include "utilities/dv_util_imgui.hpp"
#include "gui/fonts/font_fontawesome_solid.hpp"
#include "gui/fonts/font_average_mono.hpp"
#include "devue_plugin_texture.hpp"

using namespace devue;
using namespace devue::core;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_gui::dv_gui(uint32_t width, uint32_t height, const std::string& title)
    : dv_opengl_window(width, height, title), m_components(&m_systems) 
{
    m_scene_render_target = std::make_shared<dv_multisample_frame_buffer>(width, height);
}

dv_gui::~dv_gui() {}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

bool dv_gui::prepare() {
    set_theme();
    set_borderless();

    dv_util_imgui::init();

    m_custom_titlebar_height = 25;

    ImGuiIO& io = ImGui::GetIO();

    ImFontConfig config;
    config.FontDataOwnedByAtlas = false;
    config.RasterizerMultiply   = 1.2f;
    config.SizePixels           = 13.0f;

    // Add default font
    io.Fonts->AddFontFromMemoryTTF(AVERAGE_MONO, (int)AVERAGE_MONO_LEN, 13.0f, &config);

    // Add font awesome
    config.MergeMode                   = true;
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    io.Fonts->AddFontFromMemoryTTF(FONTAWESOME_SOLID, (int)FONTAWESOME_SOLID_LEN, 13.0f, &config, icon_ranges);

    // Build fonts
    io.Fonts->Build();

    // Create shaders
    try {
    	m_systems.rendering.prepare();
    }
    catch (const std::exception& e) {
    	DV_LOG("Failed to prepare rendering system. | {}", e.what());
    	return false;
    }

    try {
        m_systems.plugin.prepare();
    }
    catch (const std::exception& e) {
        DV_LOG("Failed to prepare plugin system. | {}", e.what());
        return false;
    }
    
    if (!m_systems.command.prepare(&m_components)) {
        DV_LOG("Failed to prepare command system.");
        return false;
    }
    
    devue::uuid checkerboard_uuid = create_checkerboard_texture();
    if (!checkerboard_uuid) {
        DV_LOG("Failed to create checkerboard texture.");
        return false;
    }

    m_components.checkerboard_uuid = checkerboard_uuid;

    m_systems.command.set_execute(dv_commands::flag_show_console);

    if (!m_systems.scene.prepare()) {
        DV_LOG("Failed to create scene.");
        return false;
    }

    m_systems.scene.current_scene->lighting.ambient_light.intensity     = 0.2f;
    m_systems.scene.current_scene->lighting.directional_light.intensity = 0.55f;

    dv_util_diag::init();

    glfwMaximizeWindow(m_native);
    return true;
}

void dv_gui::release() {
    // Release plugins
    m_systems.plugin.release();

    // Release systems that allocate anything in OpenGL

    // Release scenes
    m_systems.scene.release();

    // Release textures
    m_systems.texture.release();

    // Release frame buffer
    m_scene_render_target.reset();

    // Rest can be cleaned up automatically
}

void dv_gui::on_before_update() {
    dv_util_diag::update();
}

void dv_gui::on_update() {
    m_systems.scene.render_current_scene(m_scene_render_target.get());
    
    // This has to be called after scene is done rendering
    m_systems.model.remove_marked_models();
}

void dv_gui::on_after_update() {}

void dv_gui::on_gui_before_update() {
    glClearColor(0.185f, 0.185f, 0.185f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void dv_gui::on_gui_update() {
    m_skip_titlebar_hit     = ImGui::IsAnyItemHovered();
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoNav;
    flags |= ImGuiWindowFlags_NoDecoration;
    flags |= ImGuiWindowFlags_NoSavedSettings;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoDocking;
    flags |= ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});

    ImGui::Begin("Root##Window", NULL, flags);
    {
        ImGui::PopStyleVar(1);

        ImVec2 titlebar_size    = { 0.0f, 25.0f };
        ImVec2 titlebar_padding = { 0.0f, 0.0f };
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, titlebar_padding);
        ImGui::PushStyleColor(ImGuiCol_ChildBg,   0xFF1F1F1F);
        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, 0xFF1F1F1F);

        flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;

        ImGui::BeginChild("TitleBar##Window", titlebar_size, false, flags);
        {
            ImGui::PopStyleVar(1);

            auto max = ImGui::GetWindowContentRegionMax();
            auto min = ImGui::GetWindowContentRegionMin();

            ImGui::Dummy({ 0.0f, 2.0f });

            ImGui::Indent(10.0f);
            ImGui::Text("devue");

            ImGui::SameLine();

            flags = 0;
            flags |= ImGuiWindowFlags_NoNav;
            flags |= ImGuiWindowFlags_NoDecoration;
            flags |= ImGuiWindowFlags_NoSavedSettings;
            flags |= ImGuiWindowFlags_NoMove;
            flags |= ImGuiWindowFlags_NoDocking;
            flags |= ImGuiWindowFlags_NoCollapse;
            flags |= ImGuiWindowFlags_MenuBar;

            ImGui::SetCursorPosY(3.0f);

            auto avail = ImGui::GetContentRegionAvail();
            ImGui::BeginChild("TitleBarMenu##Window", { avail.x - 90.0f, 0.0f}, false, flags);
            {
                if (ImGui::BeginMenuBar()) {
                    ImGui::PushID("FileMenu");

                    if (dv_util_imgui::begin_menu("File")) {
                        if (ImGui::MenuItem("Import##MenuItem")) {
                            m_systems.command.set_execute(dv_commands::flag_show_modal_import);
                        }

                        ImGui::Separator();

                        if (ImGui::MenuItem("Exit##MenuItem")) {
                            glfwSetWindowShouldClose(m_native, 1);
                        }

                        ImGui::EndMenu();
                    }

                    ImGui::PopID();

                    ImGui::PushID("ViewMenu");

                    if (dv_util_imgui::begin_menu("View")) {
                        bool* is_executable = &m_systems.command.is_executable(dv_commands::flag_show_console);
                        ImGui::MenuItem("Console##MenuItem", "", is_executable);

                        is_executable = &m_systems.command.is_executable(dv_commands::flag_show_texture);
                        ImGui::MenuItem("Texture##MenuItem", "", is_executable);

                        ImGui::EndMenu();
                    }

                    if (dv_util_imgui::begin_menu("Plugins")) {
                        ImGui::MenuItem("Reload##MenuItem", 0, false, false);

                        ImGui::Separator();

                        bool& is_executable = m_systems.command.is_executable(dv_commands::flag_show_modal_plugins);
                        ImGui::MenuItem("View plugins##MenuItem", "", &is_executable);

                        ImGui::EndMenu();
                    }

                    if (dv_util_imgui::begin_menu("Help")) {
                        bool* is_executable = &m_systems.command.is_executable(dv_commands::flag_show_modal_about);
                        ImGui::MenuItem("About##MenuItem", "", is_executable);

                        ImGui::EndMenu();
                    }

                    ImGui::PopID();

                    ImGui::EndMenuBar();
                }
            }
            ImGui::EndChild();

            ImGui::Unindent(10.0f);

            
            ImGui::PushStyleColor(ImGuiCol_Button, 0x00FFFFFF);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, 0xFF3D3D3D);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, 0xFF3D3D3D);
            ImGui::PushStyleColor(ImGuiCol_Text, 0xFFC5C5C5);

            ImGui::SameLine(0.0f, 0.0f);
            ImGui::SetCursorPosY(min.y - titlebar_padding.y);
            if (ImGui::Button(ICON_FA_MINUS"##MinimizeButton", { 30.0f, titlebar_size.y})) {
                glfwIconifyWindow(m_native);
            }

            ImGui::SameLine(0.0f, 0.0f);
            ImGui::SetCursorPosY(min.y - titlebar_padding.y);
            if (!glfwGetWindowAttrib(m_native, GLFW_MAXIMIZED)) {
                ImGui::Button(ICON_FA_WINDOW_MAXIMIZE"##MaximizeButton", { 30.0f, titlebar_size.y });
            }
            else {
                ImGui::Button(ICON_FA_WINDOW_RESTORE"##RestoreButton", { 30.0f, titlebar_size.y });
            }

            m_hover_maximize = ImGui::IsItemHovered();

            ImGui::SameLine(0.0f, 0.0f);
            ImGui::SetCursorPosY(min.y - titlebar_padding.y);
            if (ImGui::Button(ICON_FA_XMARK"##ExitButton", { 30.0f, titlebar_size.y })) {
                glfwSetWindowShouldClose(m_native, 1);
            }

            ImGui::PopStyleColor(4);
        }
        ImGui::EndChild();

        ImGui::PopStyleColor(2);

        m_components.dockspace.render();
    }
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8.0f, 4.0f });

    m_components.hierarchy.render();
    m_components.assets.render();
    m_components.scene.render(m_scene_render_target.get());
    m_components.properties.render();
    m_components.meshes.render();

    m_systems.command.execute();

    ImGui::PopStyleVar();

    //ImGui::ShowDemoWindow();
}

void dv_gui::on_gui_after_update() {}

void dv_gui::on_resize(int width, int height) {
    if (m_scene_render_target)
    	m_scene_render_target->resize(width, height);

    if (m_systems.scene.current_scene)
    	m_systems.scene.current_scene->camera.set_aspect_ratio(static_cast<float>(width), static_cast<float>(height));
}

void dv_gui::on_scroll(double dx, double dy) {
    if (m_components.scene.is_hovered && m_systems.scene.current_scene)
    	m_systems.scene.current_scene->camera.zoom(static_cast<float>(dy));
}

void dv_gui::on_mouse_button(int btn, int action, int modifier) {

}

void dv_gui::on_mouse_move(double dx, double dy) {
    if (m_components.scene.is_hovered && m_systems.scene.current_scene) {
    	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT)) {
    		m_systems.scene.current_scene->camera.rotate(static_cast<float>(dx), static_cast<float>(dy));
    	}

    	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT)) {
    		m_systems.scene.current_scene->camera.translate(static_cast<float>(dx), static_cast<float>(dy));
    	}
    }
}

void dv_gui::on_drop(int count, const char* paths[]) {
    for (int i = 0; i < count; i++) {
        if (!m_systems.model.is_supported_file_type(paths[i])) continue;

        std::string dir = std::filesystem::path(paths[i]).remove_filename().string();
        m_systems.model.import(paths[i], dir);
    }
}

void dv_gui::set_theme() {
    ImGuiStyle& style  = ImGui::GetStyle();
    ImVec4*     colors = style.Colors;

    style.TabRounding              = 0.0f;
    style.TabBarBorderSize         = 2.0f;
    style.DockingSeparatorSize     = 1.0f;
    style.WindowBorderSize         = 0.0f;
    style.WindowMenuButtonPosition = ImGuiDir_Right;

    colors[ImGuiCol_WindowBg]             = ImVec4(0.14118f, 0.14118f, 0.14118f, 1.00f);
    colors[ImGuiCol_ChildBg]              = ImVec4(0.14118f, 0.14118f, 0.14118f, 1.00f);
    colors[ImGuiCol_PopupBg]              = ImVec4(0.14118f, 0.14118f, 0.14118f, 1.00f);
                                          
    colors[ImGuiCol_TitleBg]              = ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f);
    colors[ImGuiCol_TitleBgActive]        = ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f);
                                          
    colors[ImGuiCol_FrameBg]              = ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f);
    colors[ImGuiCol_FrameBgActive]        = ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f);
                                          
    colors[ImGuiCol_Button]               = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    colors[ImGuiCol_ButtonHovered]        = ImLerp(colors[ImGuiCol_Button], ImVec4(1.0f, 1.0f, 1.0f, 1.00f), 0.1f);
    colors[ImGuiCol_ButtonActive]         = ImLerp(colors[ImGuiCol_Button], ImVec4(0.0f, 0.0f, 0.0f, 1.00f), 0.2f);
                                          
    colors[ImGuiCol_CheckMark]            = ImVec4(0.44314f, 0.37647f, 0.90980f, 1.00f);
                                          
    colors[ImGuiCol_SliderGrab]           = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]     = ImLerp(colors[ImGuiCol_SliderGrab], ImVec4(0.0f, 0.0f, 0.0f, 1.00f), 0.2f);
                                          
    colors[ImGuiCol_Tab]                  = ImVec4(0.18039f, 0.18039f, 0.18039f, 1.00f);
    colors[ImGuiCol_TabHovered]           = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    colors[ImGuiCol_TabUnfocused]         = ImVec4(0.18039f, 0.18039f, 0.18039f, 1.00f);
    colors[ImGuiCol_TabActive]            = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]   = ImLerp(colors[ImGuiCol_TabActive], ImVec4(0.0f, 0.0f, 0.0f, 1.00f), 0.2f);
                                          
    colors[ImGuiCol_ResizeGripHovered]    = ImVec4(0.44314f, 0.37647f, 0.90980f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]     = ImVec4(0.44314f, 0.37647f, 0.90980f, 1.00f);
                                          
    colors[ImGuiCol_Header]               = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    colors[ImGuiCol_HeaderHovered]        = ImVec4(0.23922f, 0.23922f, 0.23922f, 1.00f);
    colors[ImGuiCol_HeaderActive]         = ImVec4(0.23922f, 0.23922f, 0.23922f, 1.00f);
                                          
    colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.14118f, 0.14118f, 0.14118f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.29020f, 0.24314f, 0.61176f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImLerp(colors[ImGuiCol_ScrollbarGrab], ImVec4(1.0f, 1.0f, 1.0f, 1.00f), 0.1f);
    colors[ImGuiCol_ScrollbarGrabActive]  = ImLerp(colors[ImGuiCol_ScrollbarGrab], ImVec4(0.0f, 0.0f, 0.0f, 1.00f), 0.2f);
}

devue::uuid dv_gui::create_checkerboard_texture() {
    plugins::devue_plugin_texture ptexture;
    ptexture.width      = 4;
    ptexture.height     = 4;
    ptexture.components = 3;

    ptexture.data = {
        0xAA, 0xAA, 0xAA,  0xAA, 0xAA, 0xAA,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,
        0xAA, 0xAA, 0xAA,  0xAA, 0xAA, 0xAA,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,
        0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0xAA, 0xAA, 0xAA,  0xAA, 0xAA, 0xAA,
        0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0xAA, 0xAA, 0xAA,  0xAA, 0xAA, 0xAA,
    };

    devue::uuid uuid = dv_util_uuid::create("checkerboard");

    try {
        if (m_systems.texture.textures.contains(uuid))
            return 0U;

        m_systems.texture.textures[uuid] = {
            1U,
            m_systems.texture.create_scene_texture(ptexture)
        };
    }
    catch (...) {
        return 0U;
    }

    return uuid;
}
