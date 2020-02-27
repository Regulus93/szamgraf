#version 130

in vec3 vs_out_col;
in vec3 vs_out_pos;

out vec4 fs_out_col;

void main()
{
	//végleges szín beállítása
	//fs_out_col = vec4(vs_out_col, 1);				=> def code
	//fs_out_col = vec4(1,1,1,1);					=> teljesen fehér
	//fs_out_col = vec4(vs_out_col * 2, 1);			=> fakítás, intenzítás erõsítése
	//fs_out_col = vec4(vec3(1) - vs_out_col, 1);	=> invertálás
	//discard;										=> glClearColor törlési szín kékre lett állítva

	if (vs_out_pos.x > 0) {
		fs_out_col = vec4(vec3(1) - vs_out_col, 1);
	} else {
		discard;
	}
}

// 1. feladat: rajzoljuk ki fehérrel a téglalapot!

//fs_out_col = vec4(1,1,1,1); ///teljesen fehér

// 2. feladat: uniform változók - az alkalmazás állítsa be, hogy milyen színnel töltsünk ki!

// 3. feladat: rajzoljuk ki az origó középpontú, 1 sugarú kört! Mit kell tenni, ha nem a
//    körlapot, hanem csak a körvonalat akarjuk? Eml.: discard eldobja a fragmentet

// 4. feladat: komplex számok....
