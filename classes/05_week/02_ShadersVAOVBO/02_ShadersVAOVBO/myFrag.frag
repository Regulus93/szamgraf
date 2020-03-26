#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;

out vec4 fs_out_col;

vec2 cplx_mul(vec2 u, vec2 v) {
	return vec2(u.x * v.x - u.y * v.y,
				u.x * v.y + u.y * v.x);
}

void main()
{
	vec2 c = vs_out_pos.xy;
	vec2 z = vs_out_pos.xy;

	for (int i = 1; i < 30; ++i) {
		z = cplx_mul(z, z) + c;
	}

	if (length(z) < 1)
		fs_out_col = vec4(vs_out_col, 1);
	else discard;
}

// 1. feladat: rajzoljuk ki fehérrel a téglalapot!

// 2. feladat: uniform változók - az alkalmazás állítsa be, hogy milyen színnel töltsünk ki!

// 3. feladat: rajzoljuk ki az origó középpontú, 1 sugarú kört! Mit kell tenni, ha nem a
//    körlapot, hanem csak a körvonalat akarjuk? Eml.: discard eldobja a fragmentet

// 4. feladat: komplex számok....
