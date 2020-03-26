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

// 1. feladat: rajzoljuk ki feh�rrel a t�glalapot!

// 2. feladat: uniform v�ltoz�k - az alkalmaz�s �ll�tsa be, hogy milyen sz�nnel t�lts�nk ki!

// 3. feladat: rajzoljuk ki az orig� k�z�ppont�, 1 sugar� k�rt! Mit kell tenni, ha nem a
//    k�rlapot, hanem csak a k�rvonalat akarjuk? Eml.: discard eldobja a fragmentet

// 4. feladat: komplex sz�mok....
