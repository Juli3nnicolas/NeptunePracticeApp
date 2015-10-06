#version 430 core

layout (location = 0) in vec3 vec_pos;
layout (location = 1) in vec3 vec_color;
layout (location = 2) in vec3 vec_norm;

out vec4 vert_color;

vec3 setModelPosition()
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

	// Normal settings
	vec3 norm = transpose(mv)*vec_norm;

	return norm;
}

void computeDiffuseLighting(vec3 norm)
{
	vec3 LI = vec3(1.0, 1.0, 1.0); // Light Intensity
	vec3 LIGHT_BEAM = -1*vec3(-0.1f, -1.0f, -1.0f);
	
	vec3 color = vec_color*LI;
	
	color = color * max( dot(norm, normalize(LIGHT_BEAM) ), 0.0f );
	
	vert_color = vec4(color,1.0);
}

void main()
{
	vec3 norm = setModelPosition();
	computeDiffuseLighting(norm);
}