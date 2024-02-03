#include "graphics/dv_opengl_window.hpp"
#include "gui/dv_components.hpp"
#include "systems/dv_systems.hpp"
#include "rendering/dv_render_target.hpp"

#include <memory>

namespace devue {
    class dv_gui : public devue::core::dv_opengl_window {
    public:
    	dv_gui(uint32_t width, uint32_t height, const std::string& title);
    	~dv_gui();

    private:
    	bool prepare()				override final;
        void release()              override final;

    	void on_before_update()		override final;
    	void on_update()			override final;
    	void on_after_update()		override final;
    	void on_gui_before_update()	override final;
    	void on_gui_update()		override final;
    	void on_gui_after_update()	override final;

    	void on_resize(int width, int height)					override final;
    	void on_scroll(double dx, double dy)					override final;
    	void on_mouse_button(int btn, int action, int modifier) override final;
    	void on_mouse_move(double dx, double dy)				override final;
        void on_drop(int count, const char* paths[])            override final;

        void set_theme();

    private:
    	dv_systems    m_systems;
    	dv_components m_components;
    	
    	std::shared_ptr<core::dv_render_target> m_scene_render_target = nullptr;

    private:
        devue::uuid create_checkerboard_texture();
    };
}