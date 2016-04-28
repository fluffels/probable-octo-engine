#version 330

const vec3 light = normalize(vec3(1.0f, 1.0f, 1.0f));

const float Ka = 0.5f;
const float Kd = 0.5f;

in vec3 normal;

out vec4 fragment;

void main()
{
   float intensity = Ka + Kd * dot(light, normal);

   vec3 colour = vec3(0.4, 0.4, 1.0f);

   colour = intensity * colour;
   fragment = vec4(colour, 0.7f);
}
