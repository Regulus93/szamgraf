#version 130
//preprocesszor direktíva

//OpenGL Shader nyelv

// VBO-ból érkezõ változók
//in  => storage qualifier (memóriából kap értéket a változó)

in vec3 vs_in_pos;
in vec3 vs_in_col;

// a pipeline-ban tovább adandó értékek
//out => értéket kell adnom neki, így tudom a vertextõl a fragmentig ellökni az adatot

out vec3 vs_out_pos;
out vec3 vs_out_col;

void main()
{
	//párhuzamosítás: mindegyik futáspéldányban mások a vs_in_pos, vs_in_col értékek
	//ettõl lesz áttéve homogén térbe
	gl_Position = vec4( vs_in_pos, 1 );
	vs_out_pos = vs_in_pos;
	vs_out_col = vs_in_col;
}