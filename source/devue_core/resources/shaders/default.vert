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

#version 330 core
    	
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

uniform mat4 uf_mvp;			// Model view projection matrix
uniform mat3 uf_normal_mat;		// Model normal matrix

out vec3 f_normal;
out vec2 f_uv;

void main() {
    gl_Position = uf_mvp * vec4(v_pos, 1.0);

    f_normal = normalize(uf_normal_mat * v_normal);
    f_uv	 = v_uv;
}