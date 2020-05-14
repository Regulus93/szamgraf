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
	// shaderekhez szükséges változók
	ProgramObject		m_program;			// mesh shader
	ProgramObject		m_programSkybox;	// skybox shader

	VertexArrayObject	m_CubeVao;			// VAO
	IndexBuffer			m_CubeIndices;		// index buffer
	ArrayBuffer			m_CubeVertexBuffer;	// VBO
	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;
	ArrayBuffer			m_SkyboxPos;

	//Sphere
	VertexArrayObject	m_sphereVAO;
	ArrayBuffer			m_spherePosVBO;
	IndexBuffer			m_sphereIndices;

	VertexArrayObject	m_groundVAO;
	ArrayBuffer			m_groundVBO;
	IndexBuffer			m_groundIndices;

	gCamera				m_camera;

	Texture2D			m_woodTexture;
	Texture2D			m_suzanneTexture;
	Texture2D			m_leavesTexture;
	Texture2D			m_grassTexture;
	Texture2D			m_barkTexture;

	// nyers OGL azonosítók
	GLuint				m_skyboxTexture;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	glm::vec3 m_spherePos = glm::vec3(0.0f);

	// mesh adatok
	Mesh* m_mesh;

	glm::vec3 GetPos(float u, float v);
	glm::vec3 GetNorm(float u, float v);
	glm::vec2 GetTex(float u, float v);
	const static int N = 20;
	const static int M = 20;
	glm::vec3 GetGroundPos(float u, float v);
	glm::vec3 GetGroundNorm(float u, float v);

	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitCube();
	void InitSkyBox();
	void InitSphere();
	void InitGround();

	void RenderSphere(glm::mat4 world);
	void RenderGround();
	void RenderCylinder(glm::mat4 world);
};

