#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;

out vec4 fs_out_col;

void main()
{
	//v�gleges sz�n be�ll�t�sa
	//fs_out_col = vec4(vs_out_col, 1);				=> def code
	//fs_out_col = vec4(1,1,1,1);					=> teljesen feh�r
	//fs_out_col = vec4(vs_out_col * 2, 1);			=> fak�t�s, intenz�t�s er�s�t�se
	//fs_out_col = vec4(vec3(1) - vs_out_col, 1);	=> invert�l�s
	//discard;										=> glClearColor t�rl�si sz�n k�kre lett �ll�tva

	if (vs_out_pos.x > 0) {
		fs_out_col = vec4(vec3(1) - vs_out_col, 1);
	} else {
		discard;
	}
}

// 1. feladat: rajzoljuk ki feh�rrel a t�glalapot!

//fs_out_col = vec4(1,1,1,1); ///teljesen feh�r

// 2. feladat: uniform v�ltoz�k - az alkalmaz�s �ll�tsa be, hogy milyen sz�nnel t�lts�nk ki!

// 3. feladat: rajzoljuk ki az orig� k�z�ppont�, 1 sugar� k�rt! Mit kell tenni, ha nem a
//    k�rlapot, hanem csak a k�rvonalat akarjuk? Eml.: discard eldobja a fragmentet

// 4. feladat: komplex sz�mok....
