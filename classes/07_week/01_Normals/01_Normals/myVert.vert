#version 140

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;
out vec2 vs_out_eyePos;

// shader k�ls� param�terei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;
uniform vec3 eyePos;

void main()
{
	gl_Position = MVP * vec4( vs_in_pos, 1 );

	vs_out_pos = (world * vec4(vs_in_pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
	vs_out_eyePos = eyePos;
}
