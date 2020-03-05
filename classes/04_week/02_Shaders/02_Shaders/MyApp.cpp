#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_programID = 0;
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glDepthFunc(GL_ALWAYS); //=> ilyen relációval vizsgálja meg a z értékeket(mindig a kisebb nyer), mindig a késõbbi fog látszódni

	//
	// geometria letrehozasa
	//


	std::vector<Vertex> vertices;
	vertices.push_back({ glm::vec3(-1, -1, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(1, -1, 0),  glm::vec3(0, 1, 0) });
	vertices.push_back({ glm::vec3(-1, 1, 0),  glm::vec3(0, 0, 1) });
	vertices.push_back({ glm::vec3(1, 1, 0),   glm::vec3(1, 1, 1) });

	//TRIANGLE FAN KÖZÉPPONTJA
	vertices.push_back({ glm::vec3(-0.3, 0.3, 0),   glm::vec3(1, 1, 1) });

	for (int i = 0; i < 10; ++i) {
		//sugár mérete: 0.2
		//eltolás mértéke (x tengely): -0.3
		//eltolás mértéke (y tengely): + 0.3
		vertices.push_back({
			glm::vec3(
				cos(2 * M_PI / 9 * i) * 0.2 - 0.3, //x
				sin(2 * M_PI / 9 * i) * 0.2 + 0.3, //y
				0								   //z
			)
			, glm::vec3(1,1,1) });
	}

	//TRIANGLE FAN KÖZÉPPONTJA
	vertices.push_back({ glm::vec3(0.3, 0.3, 0),   glm::vec3(1, 1, 1) });

	for (int i = 0; i < 10; ++i) {
		//sugár mérete: 0.2
		//eltolás mértéke (x tengely): -0.3
		//eltolás mértéke (y tengely): + 0.3
		vertices.push_back({
			glm::vec3(
				cos(2 * M_PI / 9 * i) * 0.2 + 0.3, //x
				sin(2 * M_PI / 9 * i) * 0.2 + 0.3, //y
				0								   //z
			)
			, glm::vec3(1,1,1) });
	}
	/*

	X irányba ugyanazt toljuk el:

	-0.50,    0, 0
	-0.50, -0.5, 0
	-0.25,    0, 0

	- 0.50 + 0.25,    0, 0
	- 0.50 + 0.25, -0.5, 0
	- 0.25 + 0.25,    0, 0

	- 0.50 + 0.50,    0, 0
	- 0.50 + 0.50, -0.5, 0
	- 0.25 + 0.50,    0, 0

	- 0.50 + 0.75,    0, 0
	- 0.50 + 0.75, -0.5, 0
	- 0.25 + 0.75,    0, 0
	*/

	vertices.push_back({ glm::vec3(-0.50,     0, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(- 0.50, -0.5, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(- 0.25,    0, 0), glm::vec3(1, 0, 0) });

	vertices.push_back({ glm::vec3(- 0.50 + 0.25,    0, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(- 0.50 + 0.25, -0.5, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(- 0.25 + 0.25,    0, 0), glm::vec3(1, 0, 0) });
														  
	vertices.push_back({ glm::vec3(- 0.50 + 0.50,    0, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(- 0.50 + 0.50, -0.5, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(- 0.25 + 0.50,    0, 0), glm::vec3(1, 0, 0) });
														
	vertices.push_back({ glm::vec3(- 0.50 + 0.75,    0, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(- 0.50 + 0.75, -0.5, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(- 0.25 + 0.75,    0, 0), glm::vec3(1, 0, 0) });

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen generált VAO beallitasa aktívnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk létre egy új VBO erõforrás nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegyük "aktívvá" a létrehozott VBO-t
	// töltsük fel adatokkal az aktív VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az aktív VBO-ba töltsünk adatokat
				  vertices.size() * sizeof(Vertex),		// ennyi bájt nagyságban
				  &vertices[0],	// errõl a rendszermemóriabeli címrõl olvasva
				  GL_STATIC_DRAW);	// úgy, hogy a VBO-nkba nem tervezünk ezután írni és minden kirajzoláskor felhasnzáljuk a benne lévõ adatokat
	

	// VAO-ban jegyezzük fel, hogy a VBO-ban az elsõ 3 float sizeof(Vertex)-enként lesz az elsõ attribútum (pozíció)
	glEnableVertexAttribArray(0); // ez lesz majd a pozíció
	glVertexAttribPointer(
		(GLuint)0,				// a VB-ben található adatok közül a 0. "indexû" attribútumait állítjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. indexû attribútum hol kezdõdik a sizeof(Vertex)-nyi területen belül
	); 

	// a második attribútumhoz pedig a VBO-ban sizeof(Vertex) ugrás után sizeof(glm::vec3)-nyit menve újabb 3 float adatot találunk (szín)
	glEnableVertexAttribArray(1); // ez lesz majd a szín
	glVertexAttribPointer(
		(GLuint)1, // !!! nvidia "hiba" javítása
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	glBindVertexArray(0); // feltöltüttük a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // feltöltöttük a VBO-t is, ezt is vegyük le

	//
	// shaderek betöltése
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket tároló program létrehozása
	m_programID = glCreateProgram();

	// adjuk hozzá a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// attributomok osszerendelese a VAO es shader kozt
	//vertex melyik csatornáját szeretnék a vertex melyik változójához bind-olni
	glBindAttribLocation( m_programID, 0, "vs_in_pos");
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illesszük össze a shadereket (kimenõ-bemenõ változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 1)
	{
		std::vector<char> ProgramErrorMessage( infoLogLength );
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
		
		char* aSzoveg = new char[ProgramErrorMessage.size()];
		memcpy( aSzoveg, &ProgramErrorMessage[0], ProgramErrorMessage.size());

		std::cout << "[app.Init()] Sáder Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	offset[0] = 0;
	offset[1] = 0;

	// mar nincs ezekre szukseg
	//törlésre történõ kijelölés: csak a program befejezõdése után fognak törlõdni (program instance élethez köti õket)
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// shader bekapcsolasa: ezzel jelezzük, hogy ezt a programunkat szeretnénk használni, ebben vannak a shaderjeink
	glUseProgram( m_programID );

	//minden rendeléskor megcsináljuk ezt
	GLuint locT = glGetUniformLocation(m_programID, "t"); //kvázi pointer, de nem gazdaságos mert mindig lekérjük a változó helyét: ezt majd initben kell megtenni
	glUniform1f(locT, SDL_GetTicks() / 1000.0); //egy elemi tömb, kvázi egy darab float
	glUniform2f(glGetUniformLocation(m_programID, "offset"), offset[0], offset[1]);

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID);

	// kirajzolás
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 11);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 11);	
	glDrawArrays(GL_TRIANGLES, 26, 12);

	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	switch (key.keysym.sym) {
	case SDLK_DOWN:
		offset[1] -= 0.01;
		break;
	case SDLK_UP:
		offset[1] += 0.01;
		break;
	case SDLK_LEFT:
		offset[0] -= 0.01;
		break;
	case SDLK_RIGHT:
		offset[0] += 0.01;
		break;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );
}