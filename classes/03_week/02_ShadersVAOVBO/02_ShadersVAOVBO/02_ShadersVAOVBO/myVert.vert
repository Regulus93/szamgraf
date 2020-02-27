#version 130
//preprocesszor direkt�va

//OpenGL Shader nyelv

// VBO-b�l �rkez� v�ltoz�k
//in  => storage qualifier (mem�ri�b�l kap �rt�ket a v�ltoz�)

in vec3 vs_in_pos;
in vec3 vs_in_col;

// a pipeline-ban tov�bb adand� �rt�kek
//out => �rt�ket kell adnom neki, �gy tudom a vertext�l a fragmentig ell�kni az adatot

out vec3 vs_out_pos;
out vec3 vs_out_col;

void main()
{
	//p�rhuzamos�t�s: mindegyik fut�sp�ld�nyban m�sok a vs_in_pos, vs_in_col �rt�kek
	//ett�l lesz �tt�ve homog�n t�rbe
	gl_Position = vec4( vs_in_pos, 1 );
	vs_out_pos = vs_in_pos;
	vs_out_col = vs_in_col;
}