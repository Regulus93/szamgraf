#version 140

// pipeline-ból bejövõ per-fragment attribútumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

// kimenõ érték - a fragment színe
out vec4 fs_out_col;

// irány fényforrás: fény iránya
uniform vec3 light_dir = vec3(-1,-1,-1);

// fénytulajdonságok: ambiens, diffúz, ...
uniform vec3 La = vec3(0.4, 0.0, 0.0);
uniform vec3 Ld = vec3(0.6, 0.6, 0.0);

// anyagtulajdonságok: ambiens, diffúz, ...
uniform vec3 Ka = vec3(1, 1, 1);
uniform vec3 Kd = vec3(1, 1, 1);

//uniform sampler2D texImage;

void main()
{	
	vec3 l = normalize(light_dir);
	
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

	vec3 n = normalize(vs_out_norm);
	vec3 toLight = -l;
	float di = clamp(dot(n, toLight), 
		0, //minimum 
		1  //maximum
		);
	vec3 diffuse = 
		Ld *
		Kd *
		di //diffúz intenzitás

	;

	//
	// fényfoltképzõ szín
	//
	/* segítség:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
	*/

	//vec3 specular = ;
	
	//
	// a fragment végsõ színének meghatározása
	//

	//fs_out_col = vec4(ambient + diffuse + specular, 1);
	fs_out_col = vec4(ambient + diffuse, 1);

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