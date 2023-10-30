#include "graphics/dv_opengl_window.hpp"

namespace devue {
	class dv_window : public devue::core::dv_opengl_window {
	public:
		dv_window(uint32_t width, uint32_t height, const std::string& title);
		~dv_window();

	private:
		void prepare()				override final;
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
	};
}