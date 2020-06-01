#version 130

out vec4 fs_out_col;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

uniform int coloring = 0;

uniform sampler2D texImage2;

void main()
{
	if (coloring == 1) {
		fs_out_col = vec4(vec3(0.1f),1);
	}
	else if (coloring == 2) {
		fs_out_col = texture(texImage2, vs_out_tex);
	} else if (coloring == 3) {
		fs_out_col = vec4(vec3(1.f),1);
	}
	
}