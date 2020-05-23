#version 330 core
#define M_PI 3.1415926535897932384626433832795

// VBO-ból érkezõ változók
in vec3 vs_in_pos; 
in vec3 vs_in_norm; 
in vec2 vs_in_tex; 

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec2 vs_out_tex;

// shader külsõ paraméterei
uniform mat4 MVP;
uniform mat4 worldIT;
uniform mat4 world;

vec3 GetPos(float u, float v)
{
	u *= float(2 * M_PI);
	v *= float(M_PI);
	float cu = cos(u), su = sin(u), cv = cos(v), sv = sin(v);
	float r = 1;

	return vec3(r * cu * sv, r * cv, r * su * sv);
}

void main()
{
	vec3 position = GetPos(vs_in_pos.x, vs_in_pos.y);
	vec3 normal = (worldIT * vec4(position, 0)).xyz;

	gl_Position = MVP * vec4( position, 1 );

	vs_out_tex = vs_in_tex;
}