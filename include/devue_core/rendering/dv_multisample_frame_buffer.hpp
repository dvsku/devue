#pragma once

#include "rendering/dv_render_target.hpp"

namespace devue::core {
	class dv_multisample_frame_buffer : public dv_render_target {
	public:
		dv_multisample_frame_buffer() = delete;
		~dv_multisample_frame_buffer();

		dv_multisample_frame_buffer(int width, int height);

		dv_multisample_frame_buffer(const dv_multisample_frame_buffer&) = delete;
		dv_multisample_frame_buffer(dv_multisample_frame_buffer&&)		= default;

		dv_multisample_frame_buffer& operator=(const dv_multisample_frame_buffer&) = delete;
		dv_multisample_frame_buffer& operator=(dv_multisample_frame_buffer&&)	   = default;

		void bind()   override final;
		void unbind() override final;

	private:
		uint32_t m_ms_fbo		= 0U;
		uint32_t m_ms_color_rbo = 0U;
		uint32_t m_ms_depth_rbo = 0U;

		uint32_t m_fbo = 0U;

	private:
		void _update_read_buffer() override final;
		void _update_draw_buffer() override final;
	};
}