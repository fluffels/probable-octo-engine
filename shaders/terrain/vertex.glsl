#version 330

layout (location=1) in vec3 position;
layout (location=2) in vec3 normal;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

out vec3 v_normal;

void main()
{
   gl_Position = projection * view * world * vec4(position, 1.0);
   v_normal = normal;
}
