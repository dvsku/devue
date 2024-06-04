#include "gui/modals/dv_modal_about.hpp"
#include "gui/dv_components.hpp"
#include "utilities/dv_util_imgui.hpp"
#include "devue_app.hpp"

using namespace devue;

dv_modal_about::dv_modal_about(dv_systems* systems, dv_components* components)
    : dv_comp(systems, components) {}

dvsku::gui_command_state dv_modal_about::render() {
    bool* visible = &m_systems->command.is_set_to_execute(dv_commands::flag_show_modal_about);

    ImVec2 pos = ImGui::GetMainViewport()->GetCenter();

    ImVec2 size = {
        ImMin(500.0f, ImGui::GetMainViewport()->Size.x * 0.85f),
        ImMin(120.0f, ImGui::GetMainViewport()->Size.y * 0.85f)
    };

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    ImGui::OpenPopup("About##Modal");
    if (ImGui::BeginPopupModal("About##Modal", visible, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::TextWrapped(DV_APP_NAME " v" DV_VERSION_STR " (" DV_ENV_STR ")");
        ImGui::Dummy({ 0.0f, 5.0f });

        ImGui::Text("Website:");
        ImGui::Indent(15.0f);
        if (dv_util_imgui::link(DV_WEBSITE)) {
            ShellExecuteA(NULL, "open", DV_WEBSITE, NULL, NULL, SW_SHOWDEFAULT);
        }
        ImGui::Unindent(15.0f);

        ImGui::EndPopup();
    }

    return dvsku::gui_command_state::repeat;
}
