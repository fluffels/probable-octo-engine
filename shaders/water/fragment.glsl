#version 330

in vec3 normal;
in vec3 view_dir;

out vec4 fragment;

uniform float K_a;
uniform float K_d;
uniform float K_s;
uniform vec3 light_dir;

const vec4 colour = vec4(0.4, 0.4, 1.0f, 0.7f);

void main()
{
    vec3 L = -1 * light_dir;
    vec3 R = reflect(L, normal);
    float I_a = K_a;

    float I_d = K_d * max(dot(light_dir, normal), 0);
    float I_s = pow(dot(R, view_dir), 100.f);
    float I = I_a + I_d + I_s;
    fragment = vec4(I * colour.rgb, colour.a);
}
