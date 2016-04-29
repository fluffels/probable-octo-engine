#version 330

in vec3 normal;

out vec4 fragment;

uniform vec3 light_dir;

const float Ka = 0.5f;
const float Kd = 0.5f;

void main()
{
   float intensity = Ka + Kd * dot(light_dir, normal);

   vec3 colour = vec3(0.4, 0.4, 1.0f);

   colour = intensity * colour;
   fragment = vec4(colour, 0.7f);
}
