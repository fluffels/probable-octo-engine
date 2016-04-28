#version 330

in vec3 v_light_dir;
in vec3 v_normal;

out vec4 colour;

const float Ka = 0.3f;
const float Kd = 2.0f;

void main()
{
    float Ia = Ka;
    float Id = Kd * max(dot(v_normal, v_light_dir), 0);

    float I = Ia + Id;
    colour = I * vec4(1.0);
}
