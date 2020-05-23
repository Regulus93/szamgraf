#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void)
{
	m_vaoID = 0;
	m_vboID = 0;
	m_ibID = 0;

	m_programID = 0;

	Cylinder cylinder = Cylinder();

}

CMyApp::~CMyApp(void)
{
}

glm::vec3 CMyApp::plainPointToParametricSurfacePoint(float u, float v )
{
	return glm::vec3(u, pow((u/2),2) - (u*v/4) + v/2 , v);
}

std::vector<glm::vec3> CMyApp::calculateDrawingPoints() {
	std::vector<glm::vec3> drawingPoints;

	for (int i = -Cylinder::N; i <= Cylinder::N; ++i) {
		for (int j = -Cylinder::M; j <= Cylinder::M; ++j)
			{
				float u = i / (float)Cylinder::N;
				float v = j / (float)Cylinder::M;

				glm::vec3 surfacePoint = plainPointToParametricSurfacePoint(u * 4,v * 4);

				if (isDrawingPoint(surfacePoint)) {	
					drawingPoints.push_back(surfacePoint);
				}
			}
		}

	return drawingPoints;
}

bool CMyApp::isDrawingPoint(glm::vec3 point) {
	return fmod(point.x, 4) == 0.0f && fmod(point.z, 4) == 0.0f;
}

bool CMyApp::Init()
{
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); 
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	Vertex vert[Cylinder::cylinder_evaluation_count + Cylinder::circle_evaluation_count];
	cylinder.InitVertexArray(vert, Cylinder::N, Cylinder::M);

	GLushort indices[Cylinder::cylinder_indices_count + Cylinder::circle_evaluation_count];
	cylinder.InitIndexArray(indices, Cylinder::N, Cylinder::M);

	cylinderFormationPositions = calculateDrawingPoints();

// [Erõforrások generálása és aktiválása]
	glGenVertexArrays(1, &m_vaoID);
	glBindVertexArray(m_vaoID);

	glGenBuffers(1, &m_vboID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
	// [VBO allokációja és struktúrájának definiálása GPU számára]
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), &vert, GL_STATIC_DRAW);

	// [VAO pozíciócsatornájának allokációja és struktúrájának definiálása GPU számára]
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// [VAO színcsatornájának allokációja és struktúrájának definiálása GPU számára]
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

// [Index Buffer definiálása]
	glGenBuffers(1, &m_ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

// [Erõforrások deaktiválása]
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

// [Shaderek fordítása]
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,		"myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER,	"myFrag.frag");

// [Fõprogram generálása és a shaderek hozzárendelése]
	m_programID = glCreateProgram();

	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

// [Fõprogram - shader változók linkelése]
	glBindAttribLocation(m_programID, 0, "vs_in_pos");
	glBindAttribLocation(m_programID, 1, "vs_in_col");

	glLinkProgram(m_programID);

	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result || infoLogLength != 0)
	{
		std::vector<char> error_massage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, error_massage.data());

		std::cout << "Hiba a shader letrehozasakor: " << error_massage.data() << std::endl;
	}
	// mar nincs ezekre szukseg
	glDeleteShader( vs_ID );
	glDeleteShader( fs_ID );

//
// egyéb inicializálás
//

	// vetítési mátrix létrehozása
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_mvp = glGetUniformLocation( m_programID, "MVP");

	return true;
}

void CMyApp::Clean()
{
	glDeleteBuffers(1, &m_vboID);
	glDeleteBuffers(1, &m_ibID);
	glDeleteVertexArrays(1, &m_vaoID);

	cylinder.~Cylinder();

	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	// nézeti transzformáció beállítása
	float t = SDL_GetTicks()/1000.0f;
	m_matView = glm::lookAt(glm::vec3(21, 18, 21),		// honnan nézzük a színteret
							glm::vec3( 0,  0,  0),		// a színtér melyik pontját nézzük
							glm::vec3( 0,  1,  0));		// felfelé mutató irány a világban
}

void CMyApp::drawFormation()
{

	float alpha = (SDL_GetTicks() / 1000.0f) * 2.0f * M_PI / translateCirclePeriodTime;
	glm::mat4 rotateTransformY = glm::rotate<float>(alpha, glm::vec3(0, 1, 0));
	glm::mat4 rotateBottomCircle = glm::rotate<float>(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	for (auto& parametricPoint : cylinderFormationPositions) {

		m_matWorld = 
			rotateTransformY
			* glm::translate<float>(glm::vec3(cylinderFormationPositions.size() + parametricPoint.x, parametricPoint.y, parametricPoint.z));

		glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &((m_matProj * m_matView * m_matWorld)[0][0]));

		glDrawElements(GL_TRIANGLES, Cylinder::cylinder_indices_count, GL_UNSIGNED_SHORT, 0);

		m_matWorld *= rotateBottomCircle;

		glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &((m_matProj * m_matView * m_matWorld)[0][0]));

		glDrawElements(GL_TRIANGLE_FAN, Cylinder::circle_evaluation_count, GL_UNSIGNED_SHORT, (GLvoid*)(2 * 1200));

		m_matWorld =  
			rotateTransformY
			* glm::translate<float>(glm::vec3(cylinderFormationPositions.size() + parametricPoint.x, parametricPoint.y+cylinder.cylinder_height, parametricPoint.z));
		
		glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &((m_matProj * m_matView * m_matWorld)[0][0]));

		glDrawElements(GL_TRIANGLE_FAN, Cylinder::circle_evaluation_count, GL_UNSIGNED_SHORT, (GLvoid*)(2 * 1200));

	}
}

/*
	Lépések:
		- puffer törlése
		- program aktiválása (m_programID)
		- VAO aktiválása
		- rajzolás
		- erõforrások deaktiválása
*/
void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram( m_programID );

	glBindVertexArray(m_vaoID);

	drawFormation();

	glBindVertexArray(0);

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
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(  45.0f,		// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									100.0f);		// tavoli vagosik
}