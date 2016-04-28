#version 330

in vec3 v_normal;

out vec4 colour;

void main()
{
   colour = vec4(v_normal, 1.0);
}
