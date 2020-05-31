#version 130

out vec4 fs_out_col;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

uniform sampler2D texImage;

void main()
{
	fs_out_col = vec4(1.f) * texture(texImage, vs_out_tex);
}