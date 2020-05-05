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
		//szín helyett textúra pozíciója!
		glm::vec2 t;
	};

	// belsõ eljárások
	GLuint GenerateRandomTexture();

	// shaderekhez szükséges változók
	GLuint m_programID; // shaderprogram erõforrás azonosító

	// OpenGL-es dolgok
	GLuint m_floor_vaoID; // vertex array object erõforrás azonosító
	GLuint m_floor_vboID; // vertex buffer object erõforrás azonosító
	GLuint m_floor_ibID;  // index buffer object erõforrás azonosító
	GLuint m_cube_vaoID;
	GLuint m_cube_vboID;
	GLuint m_cube_ibID;
	GLuint m_generatedTextureID; // generált textúra erõforrás azonosító - "padló"
	GLuint m_loadedTextureID; // betöltött textúra erõforrás azonosító - doboz

	// transzformációs mátrixok
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// uniform változók helye a shaderekben
	GLuint	m_loc_mvp;
	GLuint  m_loc_w;
	GLuint	m_loc_tex;
	GLuint	m_loc_tex2;
	GLuint	m_loc_t;

	// a jobb olvashatóság kedvéért
	void InitCube();
	void InitFloor();
	void InitShaders();
	void InitTextures();

	glm::vec3 ToDescartes(float u, float v);

	glm::vec3 m_eye = glm::vec3(0, 6, 10);
	float m_u = 0; // phi
	float m_v = 3; //theta
	//forward vector
	glm::vec3 m_fw = ToDescartes(m_u, m_v);
	//a fejemre helyezem a gömbikoordinátarendszert
	glm::vec3 m_at = m_eye + m_fw;
	bool is_mouse_pressed = false;
};

