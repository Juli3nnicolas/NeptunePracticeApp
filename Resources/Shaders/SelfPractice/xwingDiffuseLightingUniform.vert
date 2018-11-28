#version 430 core

layout (location = 0) in vec3 vec_pos;
layout (location = 1) in vec3 vec_color;
layout (location = 2) in vec3 vec_norm;

uniform mat4 MV;
uniform mat4 Proj;

out vec4 vert_color;

vec3 setModelPosition()
{
	// Position setting
	gl_Position = Proj * MV * vec4( vec_pos, 1.0 );

	// Normal settings
	vec4 norm = transpose(MV) * vec4( vec_norm, 1.0 );

	return norm.xyz;
}

void computeDiffuseLighting(vec3 norm)
{
	vec3 LI = vec3(1.0, 1.0, 1.0); // Light Intensity
	vec3 LIGHT_BEAM = -1*vec3(-0.1f, -1.0f, -1.0f);
	
	vec3 color = (vec_color/vec3(255.0f,255.0f,255.0f))*LI; // Color values are normalized (should already be normalized when loaded in the shader)
	
	color = color * max( dot(norm, normalize(LIGHT_BEAM) ), 0.0f );
	
	vert_color = vec4(color,1.0);
}

void main()
{
	vec3 norm = setModelPosition();
	computeDiffuseLighting(norm);
}