#version 130

// VBO-ból érkezõ változók
in vec3 vs_in_pos;
in vec3 vs_in_col;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_col;

//a modell koord. -ból a clipping koord.-ba (szorzás)
//C++ (CPU-ból kapjuk)
uniform mat4 world;//modell
uniform mat4 view; //view
uniform mat4 proj; //projection

void main()
{
	//oszlopok lesznek bázisok --> oszlop vektorok (jobbról szorozzuk a vertexeinkkel a mátrixainkat); a transzformációk is jobbról balra haladnak
	//a sorrend fontos, de az hogy az OpenGL hogyan végzi az nem
	gl_Position = proj * view * world * vec4(vs_in_pos, 1);
	vs_out_col = vs_in_col;
}