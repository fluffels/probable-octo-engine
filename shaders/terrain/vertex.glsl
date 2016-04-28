#version 150

in vec3 position;
in vec3 normal;

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 light_pos;

out vec3 v_normal;
out vec3 v_light_dir;

/*vec4 pick_colour(float height) {
    if (height <= 50) {
        float idx = (height / 50.0f) * 0.25;
        return texture(tex, vec2(idx, 0.25));
    } else if (height <= 75) {
        float idx = ((height - 50.0f) / 25.0f) * 0.25f + 0.25f;
        return texture(tex, vec2(idx, 0.25));
    } else if (height <= 150) {
        float idx = ((height - 75.0f) / 75.0f) * 0.25f + 0.5f;
        return texture(tex, vec2(idx, 0.75));
    } else {
        float idx = ((height - 150.0f) / 100.0f) * 0.25f + 0.75f;
        return texture(tex, vec2(idx, 0.75));
    }
}*/

void main() {
    vec4 world_pos = world * vec4(position, 1.0f);
    gl_Position = projection * view * world_pos;
    //v_colour = pick_colour(position.y);
    //v_colour = texelFetch(tex, ivec2(32, 16), 0);
    //v_colour = vec4(position.y / 200.0f);
    v_normal = normal;
    v_light_dir = normalize(light_pos - world_pos.xyz);
}
