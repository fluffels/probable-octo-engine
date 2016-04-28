#version 330

layout (location=1) in vec3 position;
layout (location=2) in vec3 normal;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light_pos;

out vec3 v_normal;
out vec3 v_light_dir;

void main()
{
    vec4 world_pos = world * vec4(position, 1.0f);
    gl_Position = projection * view * world_pos;
    v_normal = normal;
    v_light_dir = normalize(light_pos - world_pos.xyz);
}
