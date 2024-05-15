/* 
 *  Copyright (C) 2024 Nikola Stojsin - All Rights Reserved
 *  You may use, distribute and modify this code under the
 *  terms of the GPL-3.0 license.
 *  
 *  You should have received a copy of the GPL-3.0 license with
 *  this file. 
 *  If not, please visit: 
 *  	https://github.com/dvsku/devue.
*/

/*
 *  Credit to Marie-Eve Dubé
 *  Adapted from:
 *		https://asliceofrendering.com/scene%20helper/2020/01/05/InfiniteGrid/
*/

#version 330 core

layout (location = 0) in vec3 v_pos;

uniform mat4 uf_model_mat;	// Model matrix
uniform mat4 uf_view_mat;	// View matrix
uniform mat4 uf_proj_mat;	// Projection matrix

out vec3 f_near_point;
out vec3 f_far_point;

vec3 unproject_point(float x, float y, float z) {
    mat4 view_inv = inverse(uf_view_mat);
    mat4 proj_inv = inverse(uf_proj_mat);

    vec4 unprojected_point =  view_inv * proj_inv * vec4(x, y, z, 1.0);

    return unprojected_point.xyz / unprojected_point.w;
}

void main() {
    gl_Position = vec4(v_pos, 1.0);

    f_near_point = unproject_point(v_pos.x, v_pos.y, 0.0).xyz;
    f_far_point  = unproject_point(v_pos.x, v_pos.y, 1.0).xyz;
}