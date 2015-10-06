#version 430 core

layout (location = 0) in vec3 vec_pos;
layout (location = 1) in vec3 vec_color;

out vec4 vert_color;

void main()
{
	// Setting of the model-view matrix
	mat3 roty = mat3(
	sqrt(2)/2,		0,	sqrt(2)/2,
	0,				1,	0,
	-1*sqrt(2)/2,	0,	sqrt(2)/2
	);
	
	mat3 rotx = mat3(
		1,	0,			0,
		0,	sqrt(2)/2,	sqrt(2)/2,
		0,	-1*sqrt(2)/2,	sqrt(2)/2
	);
	
	mat3 mv = transpose(rotx) * transpose(roty);

	// Position setting
	gl_Position = vec4( mv* vec_pos*0.5, 1.0 );
	//gl_Position = vec4( vec_pos, 1.0);
	
	// Color settings
	vert_color = vec4(vec_color, 1.0);
}