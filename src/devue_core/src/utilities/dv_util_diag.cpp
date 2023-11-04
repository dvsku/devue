#include "utilities/dv_util_diag.hpp"
#include "glfw/glfw3.h"

using namespace devue::core;

void dv_util_diag::init() {
    m_previous_time = glfwGetTime();
}

void dv_util_diag::update() {
    double currentTime = glfwGetTime();

    dtime = currentTime - m_previous_time;
    m_previous_time = currentTime;
    m_elapsed_time += dtime;
    m_frames++;

    if (m_elapsed_time >= 1.0) {
        fps = m_frames;

        m_frames = 0U;
        m_elapsed_time = 0.0;
    }
}
