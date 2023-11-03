#version 330 core

layout (location = 0) in vec3 v_pos;

out vec3 near_point;
out vec3 far_point;

uniform mat4 mat_view;
uniform mat4 mat_proj;
uniform mat4 mat_mod;

vec3 unproject_point(float x, float y, float z, mat4 view, mat4 projection) {
	mat4 view_inv = inverse(view);
	mat4 proj_inv = inverse(projection);

	vec4 unprojected_point =  view_inv * proj_inv * vec4(x, y, z, 1.0);
	return unprojected_point.xyz / unprojected_point.w;
}

void main() {   
	near_point = unproject_point(v_pos.x, v_pos.y, 0.0, mat_view, mat_proj).xyz;
	far_point  = unproject_point(v_pos.x, v_pos.y, 1.0, mat_view, mat_proj).xyz;

	gl_Position = vec4(v_pos, 1.0);
}