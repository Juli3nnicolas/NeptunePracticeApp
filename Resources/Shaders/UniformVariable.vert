#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 i_frag_colors;

uniform mat4 rotation_matrix;

// Rotate a triangle

void main()
{
	i_frag_colors = color;
	gl_Position = rotation_matrix * vec4( position, 1.0 );
}