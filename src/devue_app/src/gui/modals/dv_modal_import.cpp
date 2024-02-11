#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "dv_modal_import.hpp"
#include "gui/modals/dv_modal_import.hpp"
#include "dv_gui_opengl/utilities/dv_util_dialog.hpp"

using namespace devue;

dv_modal_import::dv_modal_import(dv_systems* systems) 
    : m_systems(systems) {}

void dv_modal_import::set_file_path(const std::string& path) {
    m_file_path = path;

    if (!m_keep_materials) {
        m_materials_path.clear();
        m_materials_path.append(std::filesystem::path(m_file_path).remove_filename().string());
    }
}

void dv_modal_import::set_materials_path(const std::string& path) {
    m_materials_path = path;
}

bool dv_modal_import::render() {
    bool status = DV_COMMAND_REPEAT;

    ImGui::OpenPopup("Import##Popup");

    ImVec2 pos = ImGui::GetMainViewport()->GetCenter();

    ImVec2 size = {
        0.0f,
        ImMin(185.0f, ImGui::GetMainViewport()->Size.y * 0.85f)
    };

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    if (ImGui::BeginPopupModal("Import##Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar)) {
        ImGui::PushID("ImportModal");

        ImGui::Text("File");
        ImGui::SetNextItemWidth(350.0f);
        ImGui::InputText("##File", &m_file_path);

        ImGui::SameLine();
        if (ImGui::Button("Select##File", ImVec2(120, 0))) {
            auto selected = dvsku::dv_util_dialog::open_file("Import model", m_file_path, m_systems->plugin.get_model_file_types());

            // By default set texture folder to be the same
            // as the file
            if (!selected.empty())
                set_file_path(selected[0]);
        }

        ImGui::Text("Materials folder");
        ImGui::SetNextItemWidth(350.0f);
        ImGui::InputText("##Materials", &m_materials_path);

        ImGui::SameLine();
        if (ImGui::Button("Select##Materials", ImVec2(120, 0))) {
            std::string dir = dvsku::dv_util_dialog::select_dir("Select material folder", m_materials_path);

            if (!dir.empty())
                set_materials_path(dir);
        }

        ImGui::Checkbox("Remember materials folder##RMF", &m_keep_materials);

        ImVec2 size = ImGui::GetContentRegionAvail();
        ImGui::SetCursorPosX(size.x - (120 * 2) - 2.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25.0f);

        bool is_path_empty = m_file_path.empty();

        if (is_path_empty)
            ImGui::BeginDisabled();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            m_systems->model.import(m_file_path, m_materials_path);
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
    m_file_path = "";

    if (!m_keep_materials)
        m_materials_path = "";
}
