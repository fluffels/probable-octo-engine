#version 150

in vec3 v_light_dir;
in vec3 v_normal;

out vec4 colour;

uniform float K_a;
uniform float K_d;

uniform sampler2D tex;

void main()
{
    float I_a = K_a;
    float I_d = K_d * max(dot(v_normal, v_light_dir), 0);

    float I = I_a + I_d;
    colour = vec4(I * vec3(1.f), 1.0f);
    colour = texture(tex, vec2(0.5, 0.5));
}
