#include "rendering/dv_multisample_frame_buffer.hpp"
#include "glad/glad.h"

using namespace devue::core;

dv_multisample_frame_buffer::~dv_multisample_frame_buffer() {
	glDeleteFramebuffers(1, &m_fbo);

	glDeleteFramebuffers(1, &m_ms_fbo);
	glDeleteRenderbuffers(1, &m_ms_color_rbo);
	glDeleteRenderbuffers(1, &m_ms_depth_rbo);
}

dv_multisample_frame_buffer::dv_multisample_frame_buffer(int width, int height)
	: dv_render_target(width, height) {}

void dv_multisample_frame_buffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_ms_fbo);
}

void dv_multisample_frame_buffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Resolve multisample buffer to frame buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_ms_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

///////////////////////////////////////////////////////////////////////////////
// PRIVATE

void dv_multisample_frame_buffer::_update_read_buffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_ms_fbo);

	glBindRenderbuffer(GL_RENDERBUFFER, m_ms_color_rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_ms_color_rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, m_ms_depth_rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_ms_depth_rbo);
}

void dv_multisample_frame_buffer::_update_draw_buffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
}
