#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "Cylinder.h"
#include <vector>

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:

	Cylinder cylinder;

	std::vector<glm::vec3> cylinderFormationPositions;
	
	// [Shaderekhez szükséges változók]
	
	// Fõprogram azonosítója (shadereknek tudniuk kell, mely programhoz tartoznak)
	GLuint m_programID; 

	// Vertex Array Object erõforrás azonosító
	GLuint m_vaoID; 
	
	// Vertex Buffer Object erõforrás azonosító
	GLuint m_vboID; 
	
	// Index Buffer Object erõforrás azonosító
	GLuint m_ibID;  

	// Transzformációs mátrixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// Mátrixok helye a shaderekben
	GLuint	m_loc_mvp; // a három mátrixunk szorzatát adjuk át a hatékonyság érdekében

	float translateCirclePeriodTime = 9.0f;

private: 
	 bool isDrawingPoint(glm::vec3);
	 glm::vec3 plainPointToParametricSurfacePoint(float, float);
	 std::vector<glm::vec3> calculateDrawingPoints();
	 void drawFormation();
};

