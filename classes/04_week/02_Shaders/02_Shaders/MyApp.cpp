#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>


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

	// mar nincs ezekre szukseg
	//törlésre történõ kijelölés: csak a program befejezõdése után fognak törlõdni (program instance élethez köti õket)
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	m_matProj = glm::perspective(
		45.0f, //függõleges látószög fokban
		640 / 480.0f, //oldalarány, ebbõl kerül kikalkulálásra a vízszintes látószög is
		1.0f, //közeli vágósík távolsága a kamera koord. -ben a kamerától mérve
		1000.0f //távoli vágósík távolsága ....
		);

	m_loc_world = glGetUniformLocation(m_programID, "world");
	m_loc_view  = glGetUniformLocation(m_programID, "view");
	m_loc_proj  = glGetUniformLocation(m_programID, "proj");


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
	//kamera transzformációs mátrix legenerálása
	m_matView = glm::lookAt(
		glm::vec3(0, 4, 5),  //honnan nézzük
		glm::vec3(0, 0, 0),  //melyik pontját nézzük
		glm::vec3(0, 1, 0) //melyik irány van felfelé (elsõ két vektor nem határozza meg egyértelmûen a kamera irányát)
	); //hová nézünk
}


void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// shader bekapcsolasa: ezzel jelezzük, hogy ezt a programunkat szeretnénk használni, ebben vannak a shaderjeink
	glUseProgram( m_programID );

	//rotation lebegõpontos változó (másodpercenként 1-et nõjjön) => 1 mp alatt 1 teljes kör (360)
	float rot = SDL_GetTicks() / 100000.0 / 10.0f * 360;
	//forgatási szög, ezeket milyen szorzóval vegye figyelembe (itt most y tengely szerint fogja forgatni)
	m_matWorld = glm::rotate(rot, glm::vec3(0, 1, 0));

	//modellezési transzformációt átküldjük a shadernek
	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	//kamera transzformációt átküldjük a shadernek
	glUniformMatrix4fv(m_loc_view, 1, GL_FALSE, &(m_matView[0][0]));
	//projekciós transzformációt átküldjük a shadernek
	glUniformMatrix4fv(m_loc_proj, 1, GL_FALSE, &(m_matProj[0][0]));

	//shader ezeket fogja fogadni, összeszorozza az aktuálisan bejövõ vertexxel és kidobja a már clipping space-ben lévõ vertexeket

	// kapcsoljuk be a VAO-t (a VBO jön vele együtt)
	glBindVertexArray(m_vaoID);

	// kirajzolás
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// VAO kikapcsolasa
	glBindVertexArray(0);

	// shader kikapcsolasa
	glUseProgram( 0 );
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
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

	//Ablakátméretezéskor frissíteni kell a látószögeket (csonkagúlát)
	m_matProj = glm::perspective(
		45.0f, //függõleges látószög fokban (ez itt 90 fok)
		_w / (float) _h, //oldalarány, ebbõl kerül kikalkulálásra a vízszintes látószög is
		1.0f, //közeli vágósík távolsága a kamera koord. -ben a kamerától mérve
		100.0f //távoli vágósík távolsága ....
	);
}