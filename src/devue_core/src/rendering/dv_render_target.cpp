#include "rendering/dv_render_target.hpp"
#include "glad/glad.h"

using namespace devue::core;

dv_render_target::~dv_render_target() {
    glDeleteTextures(1, &m_texture);
}

dv_render_target::dv_render_target(int width, int height) {
    this->width  = width;
    this->height = height;
}

uint64_t dv_render_target::get_frame_texture() {
    return m_texture;
}

void dv_render_target::resize(int width, int height) {
    this->width = width;
    this->height = height;

    _update_read_buffer();
    _update_draw_buffer();
}
