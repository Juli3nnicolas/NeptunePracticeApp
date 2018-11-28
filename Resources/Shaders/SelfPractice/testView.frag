#version 430 core

in vec4 vert_color;
layout (location = 0) out vec4 frag_color;

void main()
{
	frag_color = vert_color;
}