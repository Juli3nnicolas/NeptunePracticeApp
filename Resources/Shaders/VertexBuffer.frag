#version 430 core

in vec3 i_frag_colors;

layout (location = 0) out vec4 o_frag_colors;

void main()
{
	o_frag_colors = vec4(i_frag_colors, 1.0);
}