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
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)
	//glDepthFunc(GL_ALWAYS); //=> ilyen rel�ci�val vizsg�lja meg a z �rt�keket(mindig a kisebb nyer), mindig a k�s�bbi fog l�tsz�dni
	//dr�tv�zas debug
	//glPolygonMode(GL_BACK, GL_LINE);

	//
	// geometria letrehozasa
	//


	std::vector<Vertex> vertices;
	vertices.push_back({ glm::vec3(1, 0, 1), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(-1, 0, -1),  glm::vec3(1, 1, 0) });
	vertices.push_back({ glm::vec3(1, 0, -1),  glm::vec3(1, 0, 1) });

	vertices.push_back({ glm::vec3(-1, 0, 1), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(-1, 0, -1),  glm::vec3(1, 1, 0) });
	vertices.push_back({ glm::vec3(1, 0, 1),  glm::vec3(1, 0, 1) });

	vertices.push_back({ glm::vec3(-1, 0, 1), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(1, 0, 1),  glm::vec3(1, 1, 0) });
	vertices.push_back({ glm::vec3(0, 2, 0),  glm::vec3(1, 0, 1) });

	vertices.push_back({ glm::vec3(1, 0, 1), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(1, 0, -1),  glm::vec3(1, 1, 0) });
	vertices.push_back({ glm::vec3(0, 2, 0),  glm::vec3(1, 0, 1) });

	vertices.push_back({ glm::vec3(1, 0, -1), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(-1, 0, -1),  glm::vec3(1, 1, 0) });
	vertices.push_back({ glm::vec3(0, 2, 0),  glm::vec3(1, 0, 1) });

	vertices.push_back({ glm::vec3(-1, 0, -1), glm::vec3(1, 0, 0) });
	vertices.push_back({ glm::vec3(-1, 0, 1),  glm::vec3(1, 1, 0) });
	vertices.push_back({ glm::vec3(0, 2, 0),  glm::vec3(1, 0, 1) });

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

	// mar nincs ezekre szukseg
	//t�rl�sre t�rt�n� kijel�l�s: csak a program befejez�d�se ut�n fognak t�rl�dni (program instance �lethez k�ti �ket)
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

	m_matProj = glm::perspective(
		45.0f, //f�gg�leges l�t�sz�g fokban
		640 / 480.0f, //oldalar�ny, ebb�l ker�l kikalkul�l�sra a v�zszintes l�t�sz�g is
		1.0f, //k�zeli v�g�s�k t�vols�ga a kamera koord. -ben a kamer�t�l m�rve
		1000.0f //t�voli v�g�s�k t�vols�ga ....
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
	//kamera transzform�ci�s m�trix legener�l�sa
	m_matView = glm::lookAt(
		glm::vec3(5, 10, 5),  //honnan n�zz�k
		glm::vec3(0, 0, 0),  //melyik pontj�t n�zz�k
		glm::vec3(0, 1, 0) //melyik ir�ny van felfel� (els� k�t vektor nem hat�rozza meg egy�rtelm�en a kamera ir�ny�t)
	); //hov� n�z�nk
}


void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// shader bekapcsolasa: ezzel jelezz�k, hogy ezt a programunkat szeretn�nk haszn�lni, ebben vannak a shaderjeink
	glUseProgram( m_programID );


	//modellez�si transzform�ci�t �tk�ldj�k a shadernek
	glUniformMatrix4fv(m_loc_proj, 1, GL_FALSE, &(m_matProj[0][0]));
	//kamera transzform�ci�t �tk�ldj�k a shadernek
	glUniformMatrix4fv(m_loc_view, 1, GL_FALSE, &(m_matView[0][0]));
	//shader ezeket fogja fogadni, �sszeszorozza az aktu�lisan bej�v� vertexxel �s kidobja a m�r clipping space-ben l�v� vertexeket



	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt)
	glBindVertexArray(m_vaoID);

	GLuint time = SDL_GetTicks() / 100;

	//10 piramis kirajzol�sa (ford�tva)
	for (int i = 0; i < 10; ++i) {
		m_matWorld =
		      glm::rotate<float>(360 / 10.0f * i, glm::vec3(0, 1, 0))                    //mindig egy piramis sorsz�m�nak megfelel� sz�mmal szorozzuk meg az elforgat�st
			* glm::rotate<float>(time / 10000 / 10.0f * 360 , glm::vec3(0, 1, 0))        //majd y tengely k�r�l elforgatjuk: azt szeretn�nk, hogy egy teljes k�rt 10 mp alatt tegyen meg
			* glm::translate<float>(glm::vec3(5, 0, 0))                                  //eltranszform�ljuk x tengely ment�n jobbra, k�rnek a sugara nagyj�b�l 5 lesz
			* glm::rotate<float>(time / 1000.0f * (2 * 6 *M_PI / 10.0)/2.0*90, glm::vec3(-1, 0, 0))//1 m�sodpercenk�nt forduljon/g�rd�lj�n (x tengely k�r�l)
			* glm::rotate<float>(90, glm::vec3(0, 0, 1))                                 //a z tengely k�r�l elforgatjuk 90 fokkal
			* glm::translate(glm::vec3(0,-1,0));                                         //letoljuk 1-gyel, hogy a testnek a k�z�ppontja nagyj�b�l az origo-ban legyen
			                                                                             //legutols�nak mindig a legels� transzform�ci�t adjuk meg

		//(2 * 6 *M_PI / 10.0)/2.0*90
		//k�r ker�lete osztva 10-el (1 mp alatt mennyit tesz meg a k�rp�ly�n) �s osztva 2-vel (oldalhossza a piramisnak) => h�ny negyedfordulatot tesz 1 mp alatt => 90-el megszorozva megkapjuk a fokbeli elfordul�s�t

			//projekci�s transzform�ci�t �tk�ldj�k a shadernek
		glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
		glDrawArrays(GL_TRIANGLES, 0, 18);
	}

	//negat�v sk�la is lesz
	m_matWorld = 
		glm::rotate<float>(time / 10000.0f * 360, glm::vec3(0, 1, 0)) //Y tengely k�r�l forgat�s=> 10 mp alatt forduljon k�rbe
		* glm::scale<float>(
		    glm::vec3(1,                             //x
			sin(time / 10000.0f * 2 * M_PI),         //Y tengely k�r�li pulz�l�s
													 //=> m�sodpercenk�nt egyet n�, majd 2 PI-vel beszorozzuk 
													 // => sin egy peri�dus�t egy mp alatt fogja megtenni 
													 // => majd beszorozzuk kett�vel, hogy 2 �s -2 k�z�tt ingadozzunk
				1                                    //z
			)
		);

	glUniformMatrix4fv(m_loc_world, 1, GL_FALSE, &(m_matWorld[0][0]));
	glDrawArrays(GL_TRIANGLES, 0, 18);

	 
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

// a k�t param�terbe az �j ablakm�ret sz�less�ge (_w) �s magass�ga (_h) tal�lhat�
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	//Ablak�tm�retez�skor friss�teni kell a l�t�sz�geket (csonkag�l�t)
	m_matProj = glm::perspective(
		45.0f, //f�gg�leges l�t�sz�g fokban (ez itt 90 fok)
		_w / (float) _h, //oldalar�ny, ebb�l ker�l kikalkul�l�sra a v�zszintes l�t�sz�g is
		1.0f, //k�zeli v�g�s�k t�vols�ga a kamera koord. -ben a kamer�t�l m�rve
		100.0f //t�voli v�g�s�k t�vols�ga ....
	);
}