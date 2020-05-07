#version 130

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D frameTex;
uniform float t;
uniform mat3 color_matrix = mat3( 0.393f, 0.349f, 0.272f, 
								  0.769f, 0.686f, 0.534f,
								  0.189f, 0.168f, 0.131f );

uniform mat3 boxFilter = mat3( -1, -1, -1, 
							   -1,  9, -1,
							   -1, -1, -1 );

uniform float texel_width;
uniform float texel_height;

void main()
{
	vec4 originalColor = texture(frameTex, vs_out_tex);

	vec4 boxColor = vec4(0);
	for	(int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			vec2 diff = vec2(
				(i - 1) * texel_width,
				(j - 1) * texel_height);

			vec4 texel = texture(frameTex, vs_out_tex + diff);

			boxColor += boxFilter[i][j] * texel;
		}

	fs_out_col = mix(originalColor, boxColor, t);
}