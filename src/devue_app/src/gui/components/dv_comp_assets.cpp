#include "gui/components/dv_comp_assets.hpp"
#include "gui/dv_components.hpp"
#include "utilities/dv_util_string.hpp"
#include "utilities/dv_util_dialog.hpp"

using namespace devue;
using namespace devue::core;

dv_comp_assets::dv_comp_assets(dv_systems* systems, dv_components* components)
    : dv_comp(systems, components) {}

bool dv_comp_assets::render() {
    ImGui::Begin("Assets##Window");

    for (auto& kvp : m_systems->model.models) {
    	dv_model& model = kvp.second;

    	ImGui::PushID(DV_FORMAT_C("{}", model.uuid));

    	if (ImGui::Selectable(DV_FORMAT_C("{}##Asset", model.name), m_components->selected.in_selected(model))) {
            m_components->selected.select(model);
    	}

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::Selectable("Add to scene##ContextMenu", false,
                ImGuiSelectableFlags_SelectOnRelease | ImGuiSelectableFlags_NoSetKeyOwner)) 
            {
                m_systems->scene.add_to_scene(model);
            }

            ImGui::EndPopup();
        }

    	ImGui::PopID();
    }

    ImGui::End();

    import_modal();
    return true;
}

void devue::dv_comp_assets::show_import_modal() {
    m_import_file_path.clear();
    m_import_textures_path.clear();
    
    m_show_import_modal = true;
}

void dv_comp_assets::import_modal() {
    if (!m_show_import_modal) return;

    ImGui::OpenPopup("Import##Popup");

    // Always center this window when appearing
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Import##Popup", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::PushID("ImportModal");

        ImGui::Text("File");
        ImGui::SetNextItemWidth(350.0f);
        ImGui::InputText("##File", &m_import_file_path);

        ImGui::SameLine();
        if (ImGui::Button("Select##File", ImVec2(120, 0))) {
            dv_util_dialog::open_file_dialog(m_import_file_path, m_systems->model.get_supported_file_types());

            // By default set texture folder to be the same
            // as the file
            if (!m_import_file_path.empty()) {
                m_import_textures_path.clear();
                m_import_textures_path.append(std::filesystem::path(m_import_file_path).remove_filename().string());
            }
        }

        ImGui::Text("Textures");
        ImGui::SetNextItemWidth(350.0f);
        ImGui::InputText("##Textures", &m_import_textures_path);

        ImGui::SameLine();
        if (ImGui::Button("Select##Textures", ImVec2(120, 0))) {

        }

        ImVec2 size = ImGui::GetContentRegionAvail();
        ImGui::SetCursorPosX(size.x - (120 * 2) - 2.0f);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25.0f);

        if (m_import_file_path.empty())
            ImGui::BeginDisabled();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            m_systems->model.import(m_import_file_path, m_import_textures_path);

            m_show_import_modal = false;
            ImGui::CloseCurrentPopup();
        }

        if (m_import_file_path.empty())
            ImGui::EndDisabled();

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine(0.0f, 10.0f);
        
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            m_show_import_modal = false;
            ImGui::CloseCurrentPopup(); 
        }
        
        ImGui::PopID();

        ImGui::EndPopup();
    }
}
