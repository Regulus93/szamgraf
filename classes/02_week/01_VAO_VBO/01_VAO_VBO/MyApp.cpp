#include "MyApp.h"
#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
}


CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	// t�rl�si sz�n legyen k�kes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f); // <-- intenz�t�s floatok (R, G, B, �tl�tszatlans�g)

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat, szinte felezi a vide�k�rtya munk�j�t, egyik legkomolyabb optimaliz�l�s
	glEnable(GL_DEPTH_TEST); // m�lys�gi teszt bekapcsol�sa (takar�s)

	//
	// geometria letrehozasa
	//

	// Geometri�nk cs�cspontjai
	Vertex vert[] =
	{ 
		//�gy az egyik h�romsz�g nem is l�tsz�dhat
		//{glm::vec3(1, -1, 0), glm::vec3(0, 1, 0)},
		//{glm::vec3(-1, -1, 0), glm::vec3(1, 0, 0)}, 
		
		/*
		-1,1,0;1,0,0
		-1,0,0;1,0,0
		 0,1,0;1,0,0

		 0,1,0;0,1,0
		 1,0,0;0,1,0
		 1,1,0;0,1,0

		-1,0, 0;
		-1,-1,0;
		 0,-1,0;

		 0,-1,0;
		 1,-1,0;
		 1,0, 0;
		*/

		//perspekt�v korrig�lt line�ris interpol�ci�

		// 1 db position		 1 db sz�n
		{glm::vec3(-1, -1, 0), glm::vec3(1, 0, 0)}, //1 db vertex
		{glm::vec3( 1, -1, 0), glm::vec3(0, 1, 0)},
		{glm::vec3(-1,  1, 0), glm::vec3(0, 0, 1)},
		{glm::vec3( 1,  1, 0), glm::vec3(1, 1, 1)}
	};


// 

	// 1 db VAO foglalasa
	glGenVertexArrays(1, &m_vaoID);
	// a frissen gener�lt VAO beallitasa akt�vnak
	glBindVertexArray(m_vaoID);
	
	// hozzunk l�tre egy �j VBO er�forr�s nevet
	glGenBuffers(1, &m_vboID); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID); // tegy�k "akt�vv�" a l�trehozott VBO-t
	// t�lts�k fel adatokkal az akt�v VBO-t
	
	//rendszer mem�ri�b�l vidd el az adatokat a vide�mem�ri�ba
	//mem�ri�t allok�l GPU mem�ri�ban
	glBufferData( GL_ARRAY_BUFFER,	// az akt�v VBO-ba t�lts�nk adatokat
				  sizeof(vert),		// ennyi b�jt nagys�gban
				  vert,	// err�l a rendszermem�riabeli c�mr�l olvasva
				  GL_STATIC_DRAW);	// �gy, hogy a VBO-nkba nem tervez�nk ezut�n �rni �s minden kirajzol�skor felhasnz�ljuk a benne l�v� adatokat
	


//---------------------------


	// VAO-ban jegyezz�k fel, hogy a VBO-ban az els� 3 float sizeof(Vertex)-enk�nt lesz az els� attrib�tum (poz�ci�)
	glEnableVertexAttribArray(0); // ez lesz majd a poz�ci� => bitm�gia

	
	glVertexAttribPointer(
		(GLuint)0,				// a VB-ben tal�lhat� adatok k�z�l a 0. "index�" attrib�tumait �ll�tjuk be -> 0-dik csatorna
		3,				// komponens szam
		GL_FLOAT,		// adatok tipusa
		GL_FALSE,		// normalizalt legyen-e
		sizeof(Vertex),	// stride (0=egymas utan) => mennyit kell ugrani a k�vetkez� komponenshez
		0				// a 0. index� attrib�tum hol kezd�dik a sizeof(Vertex)-nyi ter�leten bel�l (hol tal�lhat� a csatorna)
	); 



	//emiatt csak nvidi�n m�k�dik a projekt: AMD-Inteln�l nem tudjuk hol van a sz�n
	
	// a m�sodik attrib�tumhoz pedig a VBO-ban sizeof(Vertex) ugr�s ut�n sizeof(glm::vec3)-nyit menve �jabb 3 float adatot tal�lunk (sz�n)
	glEnableVertexAttribArray(3); // ez lesz majd a sz�n - de mi�rt 3-as attrib�tum?
	glVertexAttribPointer(
		(GLuint)3,
		3, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3)) ); //ennyit kell ugrani hogy a legels� sz�nt megtal�ljuk
		
	/*
	-----------|----------
	|	 P     |    C    |
	----------------------
	|	FFF	   | FFF     |
	|	FFF	   | FFF     |
	|	FFF	   | FFF     |
	|	FFF	   | FFF     |
	----------------------
	*/

	//0-s indexen sose foglal le az OpenGL er�forr�st, ez�rt l�nyeg�ben bekapcsoljuk
	glBindVertexArray(0); // felt�lt�tt�k a VAO-t, kapcsoljuk le (unbind)
	glBindBuffer(GL_ARRAY_BUFFER, 0); // felt�lt�tt�k a VBO-t is, ezt is vegy�k le (unbind) 

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteVertexArrays(1, &m_vaoID);
}

void CMyApp::Update()
{
}


void CMyApp::Render()
{
	// t�r�lj�k a frampuffert (GL_COLOR_BUFFER_BIT) �s a m�lys�gi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// kapcsoljuk be a VAO-t (a VBO j�n vele egy�tt) ==> VAO-hoz tartozik egy VBO, hisz valamit mindig le kell �rnia
	glBindVertexArray(m_vaoID);

	
	glDrawArrays(GL_TRIANGLE_STRIP, // kirajzol�s --> szalagszer�
		0,  // 0-dik cs�cspontt�l kezdve rajzolunk
		4); // 4 db-ot (vertex-et)

	// VAO kikapcsolasa
	glBindVertexArray(0);
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
}