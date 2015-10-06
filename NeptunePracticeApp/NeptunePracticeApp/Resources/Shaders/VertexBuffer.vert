#version 430 core

layout (location = 0) in vec3 vbo_positions;
layout (location = 1) in vec3 vbo_colors;

out vec3 i_frag_colors;

// This shader is just a pass-through

void main()
{
	i_frag_colors = vbo_colors;
	gl_Position = vec4(vbo_positions, 1.0f);
}