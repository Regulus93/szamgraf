#version 140

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
uniform vec3 eyePos;
uniform float time;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_dir = vec3(-1,-1,-1);
uniform vec3 light_dir2 = vec3(0.5f,-0.5f,-1);
uniform vec3 light_dir3 = vec3(-00.25f,-0.5f,-1);

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.4, 0.0, 0.0);
uniform vec3 Ld = vec3(0.6, 0.6, 0.0);
//fehér legyen a spekuláris fény
uniform vec3 Ls = vec3(1, 1, 1);

// anyagtulajdonságok: ambiens, diffúz, ...
uniform vec3 Ka = vec3(1, 1, 1);
uniform vec3 Kd = vec3(1, 1, 1);
//fehéren reagáljon rá a felület
uniform vec3 Ks = vec3(1, 1, 1);

//uniform sampler2D texImage;

void main()
{	
	vec3 l = normalize(light_dir);
	vec3 l2 = normalize(light_dir2);
	vec3 l3 = normalize(light_dir3);
	
	//
	// ambiens szín számítása
	//

	vec3 ambient = La * Ka;

	//
	// diffúz szín számítása
	//	
	/* segítség:
	    - normalizálás: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skaláris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
	*/

	vec3 n = normalize(vs_out_norm); //normalizáljuk hogy egységhosszú legyen a normálvektor
	vec3 toLight = -l; //fény felé mutató irányvektor
	float di = clamp(dot(n, toLight), 
		0, //minimum -> ne lehessen negatív fény
		1  //maximum -> párhuzamosak, annál nagyobb intenzitás nem lehetséges
		);
	vec3 diffuse = 
		Ld * //diffúz fényszínünk
		Kd * //diffúz fényirány
		di //diffúz intenzitás

	;

	//
	// fényfoltképzõ szín
	//
	/* segítség:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml visszaverődő r kiszámítása
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml 
	*/

	// kamerába mutató irányvektor
	vec3 c = normalize(eyePos - vs_out_pos);
	
	/*
	reflect:
		- l: beérkező fény
		- n: felületi normálvektor
	*/
	vec3 r = reflect(l, n);
	vec3 r2 = reflect(l2, n);
	vec3 r3 = reflect(l3, n);
	
	/*
	spekuláris intenzitás:
		dot(r, c) --> hogy a c és az r között mekkora a nyílásszög
		clamp(dot(r, c), 0, 1) --> ne legyen negatív fény
		pow(clamp(dot(r, c), 0, 1), 16) --> pow kell mert enyhíteni lehet a becsillanás mértékét (hajlítani a spekuláris intenzitás függvényt)
			
	*/
	float si = pow(clamp(dot(r, c), 0, 1), 64);
	float si2 = pow(clamp(dot(r2, c), 0, 1), 64);
	float si3 = pow(clamp(dot(r3, c), 0, 1), 64);
	vec3 specular = Ls * Ks * si;
	vec3 specular2 = Ls * Ks * si2;
	vec3 specular3 = Ls * Ks * si3;

	
	//
	// a fragment végsõ színének meghatározása
	//
	
	if (time > 5.0f && time < 10.f) {
		fs_out_col = vec4(ambient + diffuse + specular, 1);
	} else if ( time >= 10.f && time < 30.f ) {
		fs_out_col = vec4(ambient + diffuse + specular + specular2, 1);
	} else if ( time <= 30.f ) {
		fs_out_col = vec4(ambient + diffuse + specular + specular2 + specular3, 1);
	} else if (time < 5.0f) {
		fs_out_col = vec4(ambient + diffuse, 1);
	}


	//fs_out_col = vec4(ambient + diffuse + speculars, 1);

	// felületi normális
	//fs_out_col = vec4(vs_out_norm, 1);



	// textúrával
	//vec4 textureColor = texture(texImage, vs_out_tex);
	//fs_out_col = vec4(ambient + diffuse + specular, 1) * textureColor;
}

// Feladatok

// 1) Fényszámítás
// - ambiens
// - diffúz
// - spekuláris

// 2) Textúra
