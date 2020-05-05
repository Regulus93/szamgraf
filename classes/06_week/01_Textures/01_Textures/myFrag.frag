#version 140

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec2 vs_out_tex0;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

//textúra értéke
uniform sampler2D texImage; //mintavételező
uniform sampler2D texImage2;

uniform float t;

void main()
{
	//ő textúrát olvas: vec4-et kapunk vissza (RGB + áttetszőség)
	fs_out_col = mix(texture(texImage, vs_out_tex0),texture(texImage2, vs_out_tex0),t);
	//fs_out_col = texture(texImage, vs_out_tex0);
}

// procedurális textúra...
