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
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)
	glDepthFunc(GL_ALWAYS); //=> ilyen rel�ci�val vizsg�lja meg a z �rt�keket(mindig a kisebb nyer), mindig a k�s�bbi fog l�tsz�dni

	//
	// geometria letrehozasa
	//


	std::vector<Vertex> vertices;
	vertices.push_back({ glm::vec3(-1, -1, 0), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(1, -1, 0),  glm::vec3(0, 1, 0) });
	vertices.push_back({ glm::vec3(-1, 1, 0),  glm::vec3(0, 0, 1) });
	vertices.push_back({ glm::vec3(1, 1, 0),   glm::vec3(1, 1, 1) });

	//TRIANGLE FAN K�Z�PPONTJA
	vertices.push_back({ glm::vec3(-0.3, 0.3, 0),   glm::vec3(1, 1, 1) });

	for (int i = 0; i < 10; ++i) {
		//sug�r m�rete: 0.2
		//eltol�s m�rt�ke (x tengely): -0.3
		//eltol�s m�rt�ke (y tengely): + 0.3
		vertices.push_back({
			glm::vec3(
				cos(2 * M_PI / 9 * i) * 0.2 - 0.3, //x
				sin(2 * M_PI / 9 * i) * 0.2 + 0.3, //y
				0								   //z
			)
			, glm::vec3(1,1,1) });
	}

	//TRIANGLE FAN K�Z�PPONTJA
	vertices.push_back({ glm::vec3(0.3, 0.3, 0),   glm::vec3(1, 1, 1) });

	for (int i = 0; i < 10; ++i) {
		//sug�r m�rete: 0.2
		//eltol�s m�rt�ke (x tengely): -0.3
		//eltol�s m�rt�ke (y tengely): + 0.3
		vertices.push_back({
			glm::vec3(
				cos(2 * M_PI / 9 * i) * 0.2 + 0.3, //x
				sin(2 * M_PI / 9 * i) * 0.2 + 0.3, //y
				0								   //z
			)
			, glm::vec3(1,1,1) });
	}
	/*

	X ir�nyba ugyanazt toljuk el:

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
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
	// t�lts�k fel adatokkal az akt�v VBO-t
	glBufferData( GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
				  vertices.size() * sizeof(Vertex),		// ennyi b�jt nagys�gban
				  &vertices[0],	// err�l a rendszermem�riabeli c�mr�l olvasva
				  GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat
	

	// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); // ez lesz majd a poz�ci�
	glVertexAttribPointer(
		(GLuint)0,				// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan)
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l
	); 

	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (sz�n)
	glEnableVertexAttribArray(1); // ez lesz majd a sz�n
	glVertexAttribPointer(
		(GLuint)1, // !!! nvidia "hiba" jav�t�sa
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) );

	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le

	//
	// shaderek bet�lt�se
	//
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

	// a shadereket t�rol� program l�trehoz�sa
	m_programID = glCreateProgram();

	// adjuk hozz� a programhoz a shadereket
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// attributomok osszerendelese a VAO es shader kozt
	//vertex melyik csatorn�j�t szeretn�k a vertex melyik v�ltoz�j�hoz bind-olni
	glBindAttribLocation( m_programID, 0, "vs_in_pos");
	glBindAttribLocation( m_programID, 1, "vs_in_col");

	// illessz�k �ssze a shadereket (kimen�-bemen� v�ltoz�k �sszerendel�se stb.)
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

		std::cout << "[app.Init()] S�der Huba panasza: " << aSzoveg << std::endl;

		delete aSzoveg;
	}

	offset[0] = 0;
	offset[1] = 0;

	// mar nincs ezekre szukseg
	//t�rl�sre t�rt�n� kijel�l�s: csak a program befejez�d�se ut�n fognak t�rl�dni (program instance �lethez k�ti �ket)
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
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// shader bekapcsolasa: ezzel jelezz�k, hogy ezt a programunkat szeretn�nk haszn�lni, ebben vannak a shaderjeink
	glUseProgram( m_programID );

	//minden rendel�skor megcsin�ljuk ezt
	GLuint locT = glGetUniformLocation(m_programID, "t"); //kv�zi pointer, de nem gazdas�gos mert mindig lek�rj�k a v�ltoz� hely�t: ezt majd initben kell megtenni
	glUniform1f(locT, SDL_GetTicks() / 1000.0); //egy elemi t�mb, kv�zi egy darab float
	glUniform2f(glGetUniformLocation(m_programID, "offset"), offset[0], offset[1]);

	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	glBindVertexArray(m_vaoID);

	// kirajzol�s
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

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );
}