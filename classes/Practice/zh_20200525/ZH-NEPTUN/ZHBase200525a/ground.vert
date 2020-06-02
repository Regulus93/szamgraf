#version 130

in vec3 vs_in_pos;
in vec3 vs_in_norm;

out vec3 vs_out_norm;
out vec3 vs_out_pos;
out vec3 vs_out_col;

uniform mat4 MVP;
uniform float time;
uniform float randomNumber;

void main()
{
	vs_out_pos  = vec3(vs_in_pos.x,  sin(vs_in_pos.x * vs_in_pos.z * 100 + time) , vs_in_pos.z);  
	gl_Position = MVP * vec4(vs_out_pos, 1 );
	vs_out_norm = vs_in_norm;

	float x = pow(randomNumber,4);
	float y = sin(gl_VertexID) * randomNumber;
	float z = cos(randomNumber + gl_VertexID);

	vec3 color;
	if (z > y) {
		color = vec3(x,y,z);
	} else if (y > z) {
		color = vec3(z,y,x);
	} else if (sin(time) < 0.5) {
		color = vec3(x,y-z,z+x);
	} else {
		color = vec3(pow(x,3),y,z);
	}

	vs_out_col  = color * (gl_VertexID/40000.f) * sin(time);
}