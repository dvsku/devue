#include "gui/components/dv_comp_texture.hpp"
#include "gui/dv_components.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "utilities/dv_util_string.hpp"

using namespace devue;

dv_comp_texture::dv_comp_texture(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) {}

bool dv_comp_texture::render() {
    bool* is_executable = &m_systems->command.is_executable(dv_commands::flag_show_texture);

    std::string name = m_texture_name.empty() ? 
        "Texture###TextureWindow" : DV_FORMAT("Texture - {}###TextureWindow", m_texture_name);

    if (ImGui::Begin(name.c_str(), is_executable)) {
        if (m_texture_uuid) {
            const core::dv_scene_texture* checkerboard = m_systems->texture.get_texture(m_components->checkerboard_uuid);
            const core::dv_scene_texture* texture      = m_systems->texture.get_texture(m_texture_uuid);

            if (texture && checkerboard) {
                auto avail = ImGui::GetContentRegionAvail();

                float scale = (std::min)(avail.x / static_cast<float>(texture->width),
                    avail.y / static_cast<float>(texture->height));

                float width  = static_cast<float>(texture->width)  * scale;
                float height = static_cast<float>(texture->height) * scale;

                ImVec2 pos = {
                    ImGui::GetCursorPosX() + (avail.x - width) / 2.0f,
                    ImGui::GetCursorPosY()
                };
                
                ImGui::SetCursorPos(pos);
                ImGui::Image((void*)(intptr_t)checkerboard->texture_id, { width, height }, ImVec2(0.0f, 0.0f), ImVec2(width / 10.0f, height / 10.0f));


                ImGui::SetCursorPos(pos);
                ImGui::Image((void*)(intptr_t)texture->texture_id, { width, height }, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f));
            }
            else {
                m_texture_uuid = 0U;
                m_texture_name = "";
            }
        }
    }
    ImGui::End();

    if (*is_executable && m_requested_focus) {
        ImGuiWindow* window = ImGui::FindWindowByName("###TextureWindow");
        if (window && window->DockNode && window->DockNode->TabBar)
            window->DockNode->TabBar->NextSelectedTabId = window->TabId;

        m_requested_focus = false;
    }

    return DV_COMMAND_REPEAT;
}

void dv_comp_texture::set_texture(uuid uuid, const std::string& name) {
    m_texture_uuid = uuid;
    m_texture_name = name;
}
