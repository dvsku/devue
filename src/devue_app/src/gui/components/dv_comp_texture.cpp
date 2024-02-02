#include "gui\components\dv_comp_texture.hpp"
#include "dv_comp_texture.hpp"

using namespace devue;

dv_comp_texture::dv_comp_texture(dv_systems* systems, dv_components* components) 
    : dv_comp(systems, components) {}

bool dv_comp_texture::render() {
    bool result = DV_COMMAND_REPEAT;

    ImGui::Begin("Texture###TextureWindow");

    ImGui::End();

    return result;
}

void dv_comp_texture::set_texture(uuid id, const std::string& name) {

}
