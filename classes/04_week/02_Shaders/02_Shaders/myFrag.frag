#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;
out vec4 fs_out_col;

uniform float t; //olyan változó lesz amit a cpuról bármikor átállíthatunk

//komplex szorzás
vec2 mul( vec2 u, vec2 v ) {
	return vec2(u.x*v.x - u.y*v.y, u.x*v.y + u.y*v.x);
}

void main()
{
	vec2 z = vs_out_pos.xy; // komplex számunk: vs_out_pos.xy --> interpolációból megkapott pozíció, amit a csúcspontokból nyerünk
	vec2 c = z;

	//30 lépésben próbáljuk eldönteni, hogy konvergens-e a szám
	
	for ( int i = 0; i < 30; ++i ) {
	
		z = mul(z,z) + c;
	
	}

	if (length(z) < 1) {
		fs_out_col = vec4(vs_out_col, 1);
	} else {
		discard;
	}

	
}
