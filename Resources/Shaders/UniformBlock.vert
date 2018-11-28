#version 430 core

layout (location = 0) in vec3 vbo_pos;
layout (location = 1) in vec2 vbo_tex_pos;

out vec2 i_frag_tex_pos;

void main()
{
	i_frag_tex_pos = vbo_tex_pos;
	gl_Position = vec4(vbo_pos, 1.0);
}