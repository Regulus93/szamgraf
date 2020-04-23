#version 130

uniform mat4 mvp;

vec4 positions[6] = vec4[6](
	// 1. szakasz - x
	vec4( 0,  0, 0, 1),
	vec4( 5,  0, 0, 1),
	// 2. szakasz - y
	vec4( 0,  0, 0, 1),
	vec4( 0,  5, 0, 1),
	// 3. szakasz - z
	vec4( 0,  0, 0, 1),
	vec4( 0,  0, 5, 1)
);

vec4 colors[3] = vec4[3](
	vec4(1, 0, 0, 1), //r
	vec4(0, 1, 0, 1), //g
	vec4(0, 0, 1, 1)  //b
);

out vec4 vs_out_color;

void main()
{
	//gl_VertexID - minden példányban egyedi
	gl_Position = mvp * positions[gl_VertexID];
	vs_out_color = colors[gl_VertexID/2];
}

// 1. feladat: hogyan lehetne a positions tömb méretét lecsökkenteni 4-re? 