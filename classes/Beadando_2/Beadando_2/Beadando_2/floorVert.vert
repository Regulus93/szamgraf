#version 330 core

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos0;
in vec2 vs_in_tex0;

out vec2 vs_out_tex;

// shader k�ls� param�terei
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos0, 1 );
	
	vs_out_tex = vs_in_tex0;
}