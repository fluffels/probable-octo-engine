#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform vec3 eye;

layout (location=1) in vec3 position;

out vec4 view_dir;

void main()
{
   vec4 p = vec4(position, 1.0f);
   vec4 e = vec4(eye, 1.0f);
   vec4 wp = world * p;

   gl_Position = projection * view * wp;
   view_dir = normalize(wp - e);
}
