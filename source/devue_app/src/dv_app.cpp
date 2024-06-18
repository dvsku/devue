#include "dv_app.hpp"
#include "dv_theme.hpp"
#include "devue_plugin_texture.hpp"
#include "rendering/dv_multisample_frame_buffer.hpp"
#include "utilities/dv_util_diag.hpp"
#include "utilities/dv_util_imgui.hpp"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

using namespace devue;
using namespace devue::core;
using namespace libgui;

///////////////////////////////////////////////////////////////////////////////
// PUBLIC

dv_app::dv_app(const window_settings& settings)
    : window(settings), m_components(&m_systems)
{
    m_scene_render_target = std::make_shared<dv_multisample_frame_buffer>(settings.width, settings.height);
}

dv_app::~dv_app() {}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

bool dv_app::prepare() {
    set_borderless();
    dv_theme::set();

    // Compile shaders
    try {
        m_systems.shader.prepare();
    }
    catch (const std::exception& e) {
        DV_LOG_ERRO("", "Failed to prepare shader system. | {}", e.what());
        return false;
    }

    try {
    	m_systems.rendering.prepare();
    }
    catch (const std::exception& e) {
        DV_LOG_ERRO("", "Failed to prepare rendering system. | {}", e.what());
    	return false;
    }

    try {
        m_systems.plugin.prepare();
    }
    catch (const std::exception& e) {
        DV_LOG_ERRO("", "Failed to prepare plugin system. | {}", e.what());
        return false;
    }
    
    if (!m_systems.command.prepare(&m_components)) {
        DV_LOG_ERRO("", "Failed to prepare command system.");
        return false;
    }
    
    dvsku::uuid checkerboard_uuid = create_checkerboard_texture();
    if (!checkerboard_uuid) {
        DV_LOG_ERRO("", "Failed to create checkerboard texture.");
        return false;
    }

    m_components.checkerboard_uuid = checkerboard_uuid;

    m_systems.command.set_to_execute(dv_commands::flag_show_console);

    if (!m_systems.scene.prepare()) {
        DV_LOG_ERRO("", "Failed to create scene.");
        return false;
    }

    m_systems.scene.current_scene->lighting.ambient_light.intensity     = 0.2f;
    m_systems.scene.current_scene->lighting.directional_light.intensity = 0.55f;

    dv_util_diag::init();

    maximize_or_restore();

    return true;
}

void dv_app::release() {
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

void dv_app::on_before_update() {
    dv_util_diag::update();
}

void dv_app::on_update() {
    m_systems.scene.render_current_scene(m_scene_render_target.get());
}

void dv_app::on_after_update() {
    // This has to be called after scene is done rendering

    m_systems.scene.remove_marked_models();
    m_systems.model.remove_marked_models();
}

void dv_app::on_gui_before_update() {
    clear_frame_buffer(0.185f, 0.185f, 0.185f, 1.00f);
}

void dv_app::on_gui_update() {
    m_title_bar_hit_test    = ImGui::IsAnyItemHovered();
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

    ImRect rectangle = { {0.0f, 0.0f}, {0.0f, 0.0f} };

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

                    if (libgui::imgui::begin_menu("File")) {
                        if (ImGui::MenuItem("Import##MenuItem")) {
                            m_systems.command.set_to_execute(dv_commands::flag_show_modal_import);
                        }

                        ImGui::Separator();

                        if (ImGui::MenuItem("Exit##MenuItem")) {
                            close();
                        }

                        libgui::imgui::end_menu();
                    }

                    ImGui::PopID();

                    ImGui::PushID("ViewMenu");

                    if (libgui::imgui::begin_menu("View")) {
                        bool* is_executable = &m_systems.command.is_set_to_execute(dv_commands::flag_show_console);
                        ImGui::MenuItem("Console##MenuItem", "", is_executable);

                        is_executable = &m_systems.command.is_set_to_execute(dv_commands::flag_show_texture);
                        ImGui::MenuItem("Texture##MenuItem", "", is_executable);

                        libgui::imgui::end_menu();
                    }

                    if (libgui::imgui::begin_menu("Plugins")) {
                        ImGui::MenuItem("Reload##MenuItem", 0, false, false);

                        ImGui::Separator();

                        bool* is_executable = &m_systems.command.is_set_to_execute(dv_commands::flag_show_modal_plugins);
                        ImGui::MenuItem("View plugins##MenuItem", "", is_executable);

                        libgui::imgui::end_menu();
                    }

                    if (libgui::imgui::begin_menu("Help")) {
                        bool* is_executable = &m_systems.command.is_set_to_execute(dv_commands::flag_show_modal_about);
                        ImGui::MenuItem("About##MenuItem", "", is_executable);

                        libgui::imgui::end_menu();
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
                iconify();
            }

            ImGui::SameLine(0.0f, 0.0f);
            ImGui::SetCursorPosY(min.y - titlebar_padding.y);
            if (!is_maximized()) {
                ImGui::Button(ICON_FA_WINDOW_MAXIMIZE"##MaximizeButton", { 30.0f, titlebar_size.y });
            }
            else {
                ImGui::Button(ICON_FA_WINDOW_RESTORE"##RestoreButton", { 30.0f, titlebar_size.y });
            }

            get_title_bar().mouseover_maximize_button = ImGui::IsItemHovered();

            ImGui::SameLine(0.0f, 0.0f);
            ImGui::SetCursorPosY(min.y - titlebar_padding.y);
            if (ImGui::Button(ICON_FA_XMARK"##ExitButton", { 30.0f, titlebar_size.y })) {
                close();
            }

            ImGui::PopStyleColor(4);

            rectangle = ImGui::GetCurrentWindow()->Rect();
        }
        ImGui::EndChild();
        ImGui::PopStyleColor(2);

        get_title_bar().mouseover_title_bar = ImGui::IsMouseHoveringRect(rectangle.Min, rectangle.Max);

        m_components.dockspace.render();
    }
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8.0f, 4.0f });

    m_components.hierarchy.render();
    m_components.assets.render();
    m_components.scene.render(m_scene_render_target.get());
    m_components.properties.render();
    m_components.meshes.render();

    m_systems.command.execute_all();

    ImGui::PopStyleVar();

    //ImGui::ShowDemoWindow();
}

void dv_app::on_gui_after_update() {}

void dv_app::on_resize(int width, int height) {
    if (m_scene_render_target)
    	m_scene_render_target->resize(width, height);

    if (m_systems.scene.current_scene)
    	m_systems.scene.current_scene->camera.set_aspect_ratio(static_cast<float>(width), static_cast<float>(height));
}

void dv_app::on_scroll(double dx, double dy) {
    if (m_components.scene.is_hovered && m_systems.scene.current_scene)
    	m_systems.scene.current_scene->camera.zoom(static_cast<float>(dy));
}

void dv_app::on_mouse_button(int btn, int action, int modifier) {

}

void dv_app::on_mouse_move(double dx, double dy) {
    if (m_components.scene.is_hovered && m_systems.scene.current_scene) {
    	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT)) {
    		m_systems.scene.current_scene->camera.rotate(static_cast<float>(dx), static_cast<float>(dy));
    	}

    	if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT)) {
    		m_systems.scene.current_scene->camera.translate(static_cast<float>(dx), static_cast<float>(dy));
    	}
    }
}

void dv_app::on_drop(int count, const char* paths[]) {
    for (int i = 0; i < count; i++) {
        if (!m_systems.plugin.is_supported_model_type(paths[i])) continue;

        std::string filename = paths[i];

        if (!filename.empty()) {
            m_components.modal_import.set_file_path(filename);
            m_systems.command.set_to_execute(dv_commands::flag_show_modal_import);
            break;
        }
    }
}

dvsku::uuid dv_app::create_checkerboard_texture() {
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

    dvsku::uuid uuid = dvsku::util_uuid::create("checkerboard");

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
