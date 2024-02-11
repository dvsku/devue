#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "dv_modal_import.hpp"
#include "gui/modals/dv_modal_import.hpp"
#include "dv_gui_opengl/utilities/dv_util_dialog.hpp"

using namespace devue;

dv_modal_import::dv_modal_import(dv_systems* systems) 
    : m_systems(systems) {}

bool dv_modal_import::render() {
    bool status = DV_COMMAND_REPEAT;

    ImGui::OpenPopup("Import##Popup");

    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Import##Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::PushID("ImportModal");

        ImGui::Text("File");
        ImGui::SetNextItemWidth(350.0f);
        ImGui::InputText("##File", &m_file_path);

        ImGui::SameLine();
        if (ImGui::Button("Select##File", ImVec2(120, 0))) {
            dvsku::dv_util_dialog::open_file("Import model", m_file_path, m_systems->plugin.get_model_file_types());

            // By default set texture folder to be the same
            // as the file
            if (!m_file_path.empty()) {
                m_textures_path.clear();
                m_textures_path.append(std::filesystem::path(m_file_path).remove_filename().string());
            }
        }

        ImGui::Text("Textures");
        ImGui::SetNextItemWidth(350.0f);
        ImGui::InputText("##Textures", &m_textures_path);

        ImGui::SameLine();
        if (ImGui::Button("Select##Textures", ImVec2(120, 0))) {

        }

        ImVec2 size = ImGui::GetContentRegionAvail();
        ImGui::SetCursorPosX(size.x - (120 * 2) - 2.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25.0f);

        bool is_path_empty = m_file_path.empty();

        if (is_path_empty)
            ImGui::BeginDisabled();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            m_systems->model.import(m_file_path, m_textures_path);
            reset();
            ImGui::CloseCurrentPopup();
            status = DV_COMMAND_FINISHED;
        }

        if (is_path_empty)
            ImGui::EndDisabled();

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine(0.0f, 10.0f);

        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            reset();
            ImGui::CloseCurrentPopup();
            status = DV_COMMAND_FINISHED;
        }

        ImGui::PopID();

        ImGui::EndPopup();
    }

    return status;
}

void dv_modal_import::reset() {
    m_file_path     = "";
    m_textures_path = "";
}
