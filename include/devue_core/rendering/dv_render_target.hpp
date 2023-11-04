#pragma once

#include <cstdint>

namespace devue::core {
    class dv_render_target {
    public:
    	int width  = 0U;
    	int height = 0U;

    public:
    	dv_render_target() = delete;
    	virtual ~dv_render_target();

    	dv_render_target(int width, int height);

    	dv_render_target(const dv_render_target&) = delete;
    	dv_render_target(dv_render_target&&)	  = default;

    	dv_render_target& operator=(const dv_render_target&) = delete;
    	dv_render_target& operator=(dv_render_target&&)		 = default;

    	uint64_t get_frame_texture();

    	virtual void bind()   = 0;
    	virtual void unbind() = 0;

    	void resize(int width, int height);

    protected:
    	uint32_t m_texture = 0U;

    protected:
    	virtual void _update_read_buffer() = 0;
    	virtual void _update_draw_buffer() = 0;
    };
}