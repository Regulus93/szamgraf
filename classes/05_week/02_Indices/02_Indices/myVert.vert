#version 130

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_col;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_col;

// shader k�ls� param�terei - most a h�rom transzform�ci�s m�trixot k�l�n-k�l�n vessz�k �t
uniform mat4 MVP;

void main()
{
	//eddig 6 vertexet k�ldt�nk le �s k�l�n szoroztunk 
	//de most m�r csak egy szorz�st kell v�gezni, er�forr�sbar�tabb
	gl_Position = MVP * vec4( vs_in_pos, 1 );
	vs_out_col = vs_in_col;
}