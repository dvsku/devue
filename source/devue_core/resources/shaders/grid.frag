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
    	
in vec3 f_near_point;
in vec3 f_far_point;

uniform mat4 uf_model_mat;
uniform mat4 uf_view_mat;
uniform mat4 uf_proj_mat;

out vec4 frag_color;

vec4 grid(vec3 frag_pos, float scale, bool draw_axis) {
    vec2 coord		= frag_pos.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid		= abs(fract(coord - 0.5) - 0.5) / derivative;

    float line		= min(grid.x, grid.y);
    float minimumz	= min(derivative.y, 1);
    float minimumx	= min(derivative.x, 1);

    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));

    // z axis
    if(frag_pos.x > -1 * minimumx && frag_pos.x < 1 * minimumx)
    	color.z = 1.0;

    // x axis
    if(frag_pos.z > -1 * minimumz && frag_pos.z < 1 * minimumz)
    	color.x = 1.0;

    return color;
}

float compute_depth(vec3 frag_pos) {
    vec4 clip_space_pos = uf_proj_mat * uf_view_mat * vec4(frag_pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}

float compute_linear_depth(vec3 frag_pos) {
    float near = 0.01;
    float far  = 50.0;

    vec4  clip_space_pos   = uf_proj_mat * uf_view_mat * vec4(frag_pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0;
    float linear_depth	   = (2.0 * near * far) / (far + near - clip_space_depth * (far - near));
    
    return linear_depth / far;
}

void main() {
    float t        = -f_near_point.y / (f_far_point.y - f_near_point.y);
    vec3  frag_pos =  f_near_point + t * (f_far_point - f_near_point);

    gl_FragDepth = ((gl_DepthRange.diff * compute_depth(frag_pos)) + gl_DepthRange.near + gl_DepthRange.far) / 2.0;

    float linear_depth = compute_linear_depth(frag_pos);
    float fading	   = max(0, (0.3 - linear_depth));

    frag_color	  = (grid(frag_pos, 1, true) + grid(frag_pos, 0.1, true)) * float(t > 0);
    frag_color.a *= fading;
}