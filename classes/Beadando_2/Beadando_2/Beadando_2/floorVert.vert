#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos0;
in vec2 vs_in_tex0;

out vec2 vs_out_tex;

// shader külsõ paraméterei
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos0, 1 );
	
	vs_out_tex = vs_in_tex0;
}