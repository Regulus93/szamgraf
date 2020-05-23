#version 330 core

// pipeline-b�l bej�v� per-fragment attrib�tumok
out vec4 fs_out_col;

in vec2 vs_out_tex;

uniform sampler2D texImage;

void main()
{
	fs_out_col = vec4(1) * texture(texImage, vs_out_tex);
}