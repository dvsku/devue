#pragma once

#include "glm/vec3.hpp"
#include "glm/matrix.hpp"

namespace devue::core {
	class dv_camera {
	public:
		float speed_translate	= 0.005f;
		float speed_rotate		= 0.1f;

		glm::vec3 position = { 0.0f, 8.5f, 17.0f };

	public:
		dv_camera();
		dv_camera(const dv_camera&) = delete;
		dv_camera(dv_camera&&)		= default;

		dv_camera& operator=(const dv_camera&) = delete;
		dv_camera& operator=(dv_camera&&)	   = default;

		glm::mat4 get_view_matrix();
		glm::mat4 get_proj_matrix();

		void set_aspect_ratio(float width, float height);

		void translate(float dx, float dy);
		void rotate(float dx, float dy);
		void zoom(float dvalue);

	private:
		float m_fov			 = 45.0f;
		float m_aspect_ratio = 1920.0f / 1080.0f;

		float m_yaw   = 90.0f;
		float m_pitch = 26.0f;

		float m_min_zoom = 5.0f;
		float m_max_zoom = 110.0f;

		glm::vec3 m_target = { 0.0f, 0.0f,  0.0f };
		glm::vec3 m_front  = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_up	   = { 0.0f, 1.0f,  0.0f };

		glm::mat4 m_proj_mat{};
	};
}