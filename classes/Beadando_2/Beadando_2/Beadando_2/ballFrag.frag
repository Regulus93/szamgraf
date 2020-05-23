#version 330 core

// pipeline-ból bejövõ per-fragment attribútumok
out vec4 fs_out_col;

in vec2 vs_out_tex;

uniform sampler2D texImage;

void main()
{
	fs_out_col = vec4(1) * texture(texImage, vs_out_tex);
}