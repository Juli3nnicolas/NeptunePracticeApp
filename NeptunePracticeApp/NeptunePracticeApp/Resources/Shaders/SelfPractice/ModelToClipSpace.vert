#version 430 core

layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_colors; // Now colors arrive with a value between 0-255 and must be normalized
layout (location = 2) in vec3 in_normals;

uniform mat4 MV;
uniform mat4 PROJ;

out vec4 colors;

void main()
{
	gl_Position = PROJ * MV * vec4( in_pos, 1.0 );
	colors = vec4( in_colors, 1.0 );
}