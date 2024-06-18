#include "gui/modals/dv_modal_plugins.hpp"
#include "systems/dv_systems_bundle.hpp"
#include "utilities/dv_util_imgui.hpp"

#include <sstream>

using namespace devue;

dv_modal_plugins::dv_modal_plugins(dv_systems* systems, dv_components* components)
    : dv_comp(systems, components) {}

libgui::command::state dv_modal_plugins::render() {
    bool* visible = &m_systems->command.is_set_to_execute(dv_commands::flag_show_modal_plugins);

    ImVec2 pos = ImGui::GetMainViewport()->GetCenter();

    ImVec2 size = {
        ImMin(700.0f, ImGui::GetMainViewport()->Size.x * 0.85f), 
        ImMin(400.0f, ImGui::GetMainViewport()->Size.y * 0.85f)
    };

    ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    ImGui::OpenPopup("Plugins##Modal");
    if (ImGui::BeginPopupModal("Plugins##Modal", visible, ImGuiWindowFlags_AlwaysAutoResize)) {
        if (ImGui::IsWindowAppearing()) {
            m_current_plugin_id = 0U;
        }
        
        ImGui::PushID("PluginsModal");
        
        ImGui::PushStyleColor(ImGuiCol_ChildBg,     ImVec4(0.12157f, 0.12157f, 0.12157f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.0f, 0.0f, 0.0f, 0.00f));

        ImGui::BeginChild("PluginList", { ImGui::GetContentRegionAvail().x * 0.35f, 0.0f });
        {
            if (ImGui::BeginListBox("##PluginListBox", { -FLT_MIN, 0.0f })) {
                for (const auto& [plugin_id, plugin] : m_systems->plugin.plugins) {
                    const bool is_selected = m_current_plugin_id == plugin_id;
                    bool       is_clicked  = dv_util_imgui::selectable(plugin.name.c_str(), is_selected);

                    if (!m_current_plugin_id || is_clicked) {
                        m_current_plugin_id = plugin_id;

                        std::stringstream ss;
                        for (size_t i = 0; i < plugin.supported_model_types.size(); i++) {
                            ss << plugin.supported_model_types[i].name << " ";
                            ss << "(";
                            ss << dvsku::util_string::replace(plugin.supported_model_types[i].extensions, ";", " ");
                            ss << ")";

                            if (i != plugin.supported_model_types.size() - 1)
                                ss << "\n";
                        }

                        m_supported_model_types = ss.str();
                        
                        ss.str(std::string());
                        for (size_t i = 0; i < plugin.supported_texture_types.size(); i++) {
                            ss << plugin.supported_texture_types[i].name << " ";
                            ss << "(";
                            ss << dvsku::util_string::replace(plugin.supported_texture_types[i].extensions, ";", " ");
                            ss << ")";

                            if (i != plugin.supported_texture_types.size() - 1)
                                ss << "\n";
                        }

                        m_supported_texture_types = ss.str();
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndListBox();
            }
        }
        ImGui::EndChild();

        ImGui::PopStyleColor(2);

        ImGui::SameLine(0.0f, 10.0f);

        ImGui::BeginChild("PluginInfo", { ImGui::GetContentRegionAvail().x, 0.0f });
        {
            if (m_current_plugin_id && m_systems->plugin.plugins.contains(m_current_plugin_id)) {
                const auto& plugin = m_systems->plugin.plugins[m_current_plugin_id];
                const float indent = 15.0f;

                ImGui::Text("Name:");
                ImGui::Indent(indent);
                ImGui::TextWrapped(plugin.name.c_str());
                ImGui::Unindent(indent);
                ImGui::Dummy({ 0.0f, 5.0f });

                ImGui::Text("File name:");
                ImGui::Indent(indent);
                if (dv_util_imgui::link(plugin.filename.c_str())) {
                    auto path = std::filesystem::current_path() / "plugins";

                    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
                        ShellExecuteA(NULL, "open", path.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
                    }
                }
                ImGui::Unindent(indent);
                ImGui::Dummy({ 0.0f, 5.0f });

                if (!plugin.version.empty()) {
                    ImGui::Text("Version:");
                    ImGui::Indent(indent);
                    ImGui::TextWrapped(plugin.version.c_str());
                    ImGui::Unindent(indent);
                    ImGui::Dummy({ 0.0f, 5.0f });
                }
                
                if (!plugin.author.empty()) {
                    ImGui::Text("Author:");
                    ImGui::Indent(indent);
                    ImGui::TextWrapped(plugin.author.c_str());
                    ImGui::Unindent(indent);
                    ImGui::Dummy({ 0.0f, 5.0f });
                }

                if (!plugin.website.empty()) {
                    ImGui::Text("Website:");
                    ImGui::Indent(indent);
                    if (dv_util_imgui::link(plugin.website.c_str())) {
                        ShellExecuteA(NULL, "open", plugin.website.c_str(), NULL, NULL, SW_SHOWDEFAULT);
                    }
                    ImGui::Unindent(indent);
                    ImGui::Dummy({ 0.0f, 5.0f });
                }

                if (plugin.supported_model_types.size() && !m_supported_model_types.empty()) {
                    ImGui::Text("Supported model types:");
                    ImGui::Indent(indent);
                    ImGui::TextWrapped(m_supported_model_types.c_str());
                    ImGui::Unindent(indent);
                    ImGui::Dummy({ 0.0f, 5.0f });
                }

                if (plugin.supported_texture_types.size() && !m_supported_texture_types.empty()) {
                    ImGui::Text("Supported texture types:");
                    ImGui::Indent(indent);
                    ImGui::TextWrapped(m_supported_texture_types.c_str());
                    ImGui::Unindent(indent);
                }
            }
        }
        ImGui::EndChild();

        ImGui::PopID();
        ImGui::EndPopup();
    }

    return libgui::command::state::repeat;
}
