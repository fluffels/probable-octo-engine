#version 420

layout (location=1) in vec2 position;
layout (location=2) in vec3 colour;

out vec3 v_colour;

void main()
{
   gl_Position = vec4(position, 0.0, 1.0);
   v_colour = colour;
}
