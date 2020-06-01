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

// mesh
#include "includes/ObjParser_OGL3.h"

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
	ProgramObject		m_MeshProgram;		// mesh shader
	ProgramObject		m_SkyboxProgram;	// skybox shader
	ProgramObject		m_AxesProgram;		// axes shader
	ProgramObject		groundProgram;		
	ProgramObject		cubeProgram;		
	ProgramObject		sphereProgram;		
	ProgramObject		ringProgram;		

	// kocka
	VertexArrayObject	m_CubeVao;			// VAO
	IndexBuffer			m_CubeIndices;		// index buffer
	ArrayBuffer			m_CubeVertexBuffer;	// VBO
	// skybox kocka
	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;
	// gömb
	const int groundN = 200;
	const int groundM = 200;
	const int N = 20;
	const int M = 20;
	VertexArrayObject	m_SphereVao;
	IndexBuffer			m_SphereIndices;
	ArrayBuffer			m_SphereVertexBuffer;

	// tórusz zongora
	VertexArrayObject	ringVao;
	IndexBuffer			ringInd;
	ArrayBuffer			ringVbo;

	// színpad
	VertexArrayObject	groundVao;			
	IndexBuffer			groundInd;		
	ArrayBuffer			groundVbo;	

	gCamera				m_camera;

	Texture2D			m_WoodTexture;
	Texture2D			m_MarronTexture;
	Texture2D			m_EarthTexture;

	Texture2D			taylorTexture;
	Texture2D			dancerTexture;
	Texture2D			taylorDressTexture;
	Texture2D			dancerDressTexture;

	Texture2D			pianoTexture;


	// nyers OGL azonosítók
	GLuint				m_skyboxTexture = 0;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	// mesh adatok
	Mesh *m_SuzanneMesh = nullptr;
	Mesh *pianoMesh = nullptr;

	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitCube();
	void InitSphere();
	void InitSkyBox();

	bool movingStage = false;
	bool choreoIsOn = true;
	
	void RenderCube(glm::mat4 transformations);
	void RenderSphere(glm::mat4 transformations);

	void InitGround();
	glm::vec3 GetGroundPos(float u, float v);
	void RenderGround();

	void InitRing();
	glm::vec3 CMyApp::GetRingPos(float u, float v);
	glm::vec3 CMyApp::GetRingNorm(float u, float v);
	void RenderPiano(glm::vec3 pos);

	void RenderStage();
	void RenderMeshPiano(glm::vec3 pos);

	void RenderPerson(glm::vec3 pos, bool isTaylor);
	void RenderBody(glm::vec3 pos, bool isTaylor);
	void RenderHead(glm::vec3 pos, bool isTaylor);

	glm::vec3 Eval(float t);
	glm::vec3 GetChoreoPos(glm::vec3 pos, float time, bool isTaylor);
	std::vector<glm::vec3> m_controlPoints{ 
		  {glm::vec3(0.f, 0.f, 0.f)}
		, {glm::vec3(0.f, 2.5f, 2.f)}
		, {glm::vec3(0.f, -5.f, 10.f)}
	
	};

};

