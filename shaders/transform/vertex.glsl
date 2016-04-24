#version 330

layout (location=1) in vec3 position;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

out vec3 v_colour;

void main()
{
   gl_Position = projection * view * world * vec4(position, 1.0);
}
