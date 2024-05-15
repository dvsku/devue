#include "scene/dv_camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

using namespace devue::core;
using namespace dvsku;

dv_camera::dv_camera() {
    uuid = dv_util_uuid::create();

    // Call transformations to apply initial values
    translate(0.0f, 0.0f);
    rotate(0.0f, 0.0f);
    zoom(-30.0f);
}

glm::mat4 dv_camera::get_view_matrix() {
    return glm::lookAt(position, position + m_front, m_up);
}

glm::mat4 dv_camera::get_proj_matrix() {
    return glm::perspective(glm::radians(m_fov), m_aspect_ratio, 0.1f, 500.0f);
}

void dv_camera::set_aspect_ratio(float width, float height) {}

void dv_camera::translate(float dx, float dy) {
    glm::vec3 cameraRight = glm::normalize(glm::cross(m_front, m_up));
    
    position -= dx * speed_translate * cameraRight;
    m_target -= dx * speed_translate * cameraRight;

    position -= dy * speed_translate * m_up;
    m_target -= dy * speed_translate * m_up;
}

void dv_camera::rotate(float dx, float dy) {
    m_yaw   += dx * speed_rotate;
    m_pitch -= dy * speed_rotate;

    if (m_pitch >  89.0f) m_pitch =  89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    float distance = glm::length(position - m_target);
    position.x = m_target.x + distance * cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    position.y = m_target.y + distance * sin(glm::radians(m_pitch));
    position.z = m_target.z + distance * sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(m_target - position);
}

void dv_camera::zoom(float dvalue) {
    float distance = glm::length(position - m_target);

    bool zoom_blocked = (static_cast<int>(distance) >= m_max_zoom && dvalue < 0) ||
    	(static_cast<int>(distance) <= m_min_zoom && dvalue > 0);

    if (zoom_blocked) return;

    distance -= dvalue;

    glm::vec3 direction = glm::normalize(position - m_target);
    position = m_target + direction * distance;
}
