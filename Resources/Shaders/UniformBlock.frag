#version 430 core

in vec2 i_frag_tex_pos;
out vec4 o_frag_color;

layout (binding = 0) uniform BlobSettings
{
	vec4 inner_color;
	vec4 outer_color;
	float radius_inner;
	float radius_outer;
};


void main()
{
	float dx = i_frag_tex_pos.x - 0.5;
	float dy = i_frag_tex_pos.y - 0.5;
	float dist = sqrt( dx*dx + dy*dy );
	
	o_frag_color = mix( inner_color, outer_color, smoothstep(radius_inner, radius_outer, dist) );
}