#version 330 core

in vec3 f_normal;
in vec2 f_uv;

uniform sampler2D sampler;
uniform vec3 ambient_light;
uniform ivec3 dir_light_dir;
uniform vec3 dir_light_color;

out vec4 frag_color;

void main() {
    float diffuse_factor	= max(dot(f_normal, -dir_light_dir), 0.0);
    vec3 diffuse_lighting	= diffuse_factor * dir_light_color;
    vec3 combined_color		= (ambient_light + diffuse_lighting);

    frag_color = texture(sampler, f_uv) * vec4(combined_color, 1.0);
}