#version 130

out vec4 fs_out_col;
in vec3 vs_out_norm;
in vec2 vs_out_tex;
in vec3 vs_out_pos;

uniform int coloring = 0;


// f�nytulajdons�gok: ambiens, diff�z, ...
uniform vec4 La = vec4(0.4, 0, 0.0,1);
uniform vec4 Ld = vec4(0, 0, 1,1);
//feh�r legyen a spekul�ris f�ny
uniform vec4 Ls = vec4(1, 1, 1,1);

// anyagtulajdons�gok: ambiens, diff�z, ...
uniform vec4 Ka = vec4(1, 1, 1,1);
uniform vec4 Kd = vec4(1, 1, 1,1);
//feh�ren reag�ljon r� a fel�let
uniform vec4 Ks = vec4(1, 1, 1,1);
uniform vec4 Ks2 = vec4(0, 0, 1,1);
uniform vec4 Ks3 = vec4(1, 0, 0,1);
uniform vec4 Ks4 = vec4(1, 1, 0,1);

// f�nytulajdons�gok
uniform vec3 light_pos = vec3( 6, 2, 0 );
uniform vec3 light_pos2 = vec3( 1, 5, 5 );
uniform vec3 light_pos3 = vec3( -7, 2, -2 );
//sz�nt�rtulajdons�gok
uniform vec3 eyePos;
uniform float specular_power = 24;



uniform sampler2D texImage2;

void main()
{

//
	// ambiens sz�n sz�m�t�sa
	//
	vec4 ambient = La * Ka;

	//
	// diff�z sz�n sz�m�t�sa
	//

	/* seg�ts�g:
		- normaliz�l�s: http://www.opengl.org/sdk/docs/manglsl/xhtml/normalize.xml
	    - skal�ris szorzat: http://www.opengl.org/sdk/docs/manglsl/xhtml/dot.xml
	    - clamp: http://www.opengl.org/sdk/docs/manglsl/xhtml/clamp.xml
	*/
	vec3 normal = normalize( vs_out_norm );
	vec3 toLight = normalize(light_pos - vs_out_pos);
	vec3 toLight2 = normalize(light_pos2 - vs_out_pos);
	vec3 toLight3 = normalize(light_pos3 - vs_out_pos);
	float di = clamp( dot( toLight, normal), 0.0f, 1.0f );
	vec4 diffuse = vec4(Ld.rgb*Kd.rgb*di, Kd.a);

	//
	// f�nyfoltk�pz� sz�n
	//

	/* seg�ts�g:
		- reflect: http://www.opengl.org/sdk/docs/manglsl/xhtml/reflect.xml
		- power: http://www.opengl.org/sdk/docs/manglsl/xhtml/pow.xml
	*/
	vec4 specular = vec4(0);
	vec4 specular2 = vec4(0);
	vec4 specular3 = vec4(0);

	if ( di > 0 )
	{
		vec3 e = normalize( eyePos - vs_out_pos );
		vec3 r = reflect( -toLight, normal );
		vec3 r2 = reflect( -toLight2, normal );
		vec3 r3 = reflect( -toLight3, normal );
		float si = pow( clamp( dot(e, r), 0.0f, 1.0f ), specular_power );
		float si2 = pow( clamp( dot(e, r2), 0.0f, 1.0f ), specular_power );
		float si3 = pow( clamp( dot(e, r3), 0.0f, 1.0f ), specular_power );
		specular = Ls*Ks*si;
		specular2 = Ls*Ks2*si2;
		specular3 = Ls*Ks3*si3;
	}

	if (coloring == 1) {
		fs_out_col = (ambient + diffuse + specular + specular2 + specular3 ) * vec4(vec3(0.1f),1);
	}
	else if (coloring == 2) {
		fs_out_col = texture(texImage2, vs_out_tex);
	} else if (coloring == 3) {
		fs_out_col = vec4(vec3(1.f),1);
	}
	
}