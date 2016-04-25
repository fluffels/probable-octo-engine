#version 330

layout(binding=8) uniform samplerCube cubeMap;

in vec4 view_dir;

out vec4 fragment;

void main()
{
   fragment = texture(cubeMap, view_dir.xyz);
}

