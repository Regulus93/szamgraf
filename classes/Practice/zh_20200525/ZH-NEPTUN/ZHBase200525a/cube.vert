#version 130

in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

out vec2 vs_out_tex;
out vec3 vs_out_norm;
out vec3 vs_out_pos;

uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );
	vs_out_norm = vs_in_norm;
	vs_out_tex = vs_in_tex;
	vs_out_pos = vs_in_pos;
}