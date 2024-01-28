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

in vec3 f_normal;
in vec2 f_uv;

uniform sampler2D uf_sampler;	// Texture sample
uniform float	  uf_textured;	// Is textured or not
uniform vec3	  uf_col;		// Color
uniform vec3	  uf_al_col;	// Ambient light color
uniform ivec3	  uf_dl_dir;	// Directional light direction
uniform vec3	  uf_dl_col;	// Directional light color

out vec4 frag_color;

void main() {
	vec4 base_col = (uf_textured == 0.0f) ? vec4(uf_col, 1.0) : texture(uf_sampler, f_uv);
	
    float diffuse_factor   = max(dot(f_normal, -uf_dl_dir), 0.0);
    vec3  diffuse_lighting = diffuse_factor * uf_dl_col;
	vec3  lighting		   = uf_al_col + diffuse_lighting;

    frag_color = base_col * vec4(lighting, 1.0);
}