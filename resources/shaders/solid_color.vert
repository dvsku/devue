#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;

uniform mat4 mvp;
uniform mat3 normal_mat;

out vec3 f_normal;

void main() {
	gl_Position = mvp * vec4(v_pos, 1.0);

	f_normal	= normalize(normal_mat * v_normal);
}