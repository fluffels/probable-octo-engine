#version 150

in vec4 v_colour;
in vec3 v_light_dir;
in vec3 v_normal;

out vec4 fragment;

uniform float K_a;
uniform float K_d;

void main()
{
    float I_a = K_a;
    float I_d = K_d * max(dot(v_normal, v_light_dir), 0);
    float I = I_a + I_d;

    fragment = vec4(I * v_colour.rgb, v_colour.a);
}
