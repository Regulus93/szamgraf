#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;
out vec4 fs_out_col;

uniform float t; //olyan v�ltoz� lesz amit a cpur�l b�rmikor �t�ll�thatunk

//komplex szorz�s
vec2 mul( vec2 u, vec2 v ) {
	return vec2(u.x*v.x - u.y*v.y, u.x*v.y + u.y*v.x);
}

void main()
{
	vec2 z = vs_out_pos.xy; // komplex sz�munk: vs_out_pos.xy --> interpol�ci�b�l megkapott poz�ci�, amit a cs�cspontokb�l nyer�nk
	vec2 c = z;

	//30 l�p�sben pr�b�ljuk eld�nteni, hogy konvergens-e a sz�m
	
	for ( int i = 0; i < 30; ++i ) {
	
		z = mul(z,z) + c;
	
	}

	if (length(z) < 1) {
		fs_out_col = vec4(vs_out_col, 1);
	} else {
		discard;
	}

	
}
