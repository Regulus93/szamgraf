#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

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

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	ProgramObject		m_floorProgram;
	ProgramObject		m_ballProgram;
	ProgramObject		m_programSkybox;
	ProgramObject		m_axesProgram;
	ProgramObject		m_particleProgram;

	VertexArrayObject	m_floorVAO;
	ArrayBuffer			m_floorPos;
	ArrayBuffer			m_floorTex;
	IndexBuffer			m_floorIndices;

	VertexArrayObject	m_ballVAO;
	ArrayBuffer			m_ballVBO;
	IndexBuffer			m_ballIndices;

	VertexArrayObject	m_gpuParticleVAO;
	ArrayBuffer			m_gpuParticleBuffer;
	ArrayBuffer			m_gpuParticleVelBuffer;

	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;

	Texture2D			m_floorTexture;
	Texture2D			m_ballTexture;
	GLuint				m_skyboxTexture;

	gCamera				m_camera;

	//Racspontok reszletessege
	const static int N = 20;
	const static int M = 20;

	//Segedvonalak megjelenitese
	bool axesShown = true;

	//Fek allapotjelzo
	bool breakIsOn = false;

	//Labda tulajdonsagai
	float ballVelocity = 0.25f;
	glm::vec3 ballWorld = glm::vec3(5,1,5);

	//Reszecskerendszer tulajdonsagai
	int		m_particleCount = 100;
	float		particleSize = 0.1f;

	std::vector<glm::vec3>		m_particlePos{};
	std::vector<glm::vec3>		m_particleVel{};

	//textura inicializalasa
	glm::vec2 CMyApp::GetTex(float u, float v);

	//Eroforrasok inicializalasa es linkelese
	void InitShaders();

	//Geometria inicializalasa
	void InitFloor();
	void InitBall();
	void InitSkyBox();
	void InitParticles();

	//Rajzolasi funkciok
	void RenderFloor();
	void RenderAxes();
	void RenderBall(glm::mat4 viewProj);
	void RenderParticles();
	
	void RenderSkybox();
};

