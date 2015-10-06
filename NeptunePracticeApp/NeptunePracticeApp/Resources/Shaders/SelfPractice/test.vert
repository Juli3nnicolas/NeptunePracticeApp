#version 430 core

layout (location = 0) in vec3 vert_pos;

out vec4 vert_color;

void main()
{
	// Each coordinate has its value doubled by the shader
	// Then, each coordinate is divided by 4 during the
	// projection division (before the clipping stage)
	gl_Position = vec4(vert_pos*2, 4.0);
	
	if (vert_pos.x != 0 || vert_pos.y != 0 || vert_pos.z != 0)
		vert_color = vec4(vert_pos, 1.0);
	else
		vert_color = vec4(1.0, 1.0, 1.0, 1.0);
}