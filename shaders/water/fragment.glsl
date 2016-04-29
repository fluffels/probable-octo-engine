#version 330

in vec3 normal;

out vec4 fragment;

uniform float K_a;
uniform float K_d;
uniform vec3 light_dir;

const vec4 colour = vec4(0.4, 0.4, 1.0f, 0.7f);

void main()
{
   float I = K_a + K_d * dot(light_dir, normal);

   fragment = vec4(I * colour.rgb, colour.a);
}
