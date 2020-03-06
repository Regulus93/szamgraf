#version 130

// VBO-b�l �rkez� v�ltoz�k
in vec3 vs_in_pos;
in vec3 vs_in_col;

// a pipeline-ban tov�bb adand� �rt�kek
out vec3 vs_out_pos;
out vec3 vs_out_col;

//a modell koord. -b�l a clipping koord.-ba (szorz�s)
//C++ (CPU-b�l kapjuk)
uniform mat4 world;//modell
uniform mat4 view; //view
uniform mat4 proj; //projection

void main()
{
	//oszlopok lesznek b�zisok --> oszlop vektorok (jobbr�l szorozzuk a vertexeinkkel a m�trixainkat); a transzform�ci�k is jobbr�l balra haladnak
	//a sorrend fontos, de az hogy az OpenGL hogyan v�gzi az nem
	gl_Position = proj * view * world * vec4(vs_in_pos, 1);
	vs_out_col = vs_in_col;
}