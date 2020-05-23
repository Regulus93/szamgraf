#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <random>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

CMyApp::~CMyApp(void)
{
}

bool CMyApp::Init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	InitShaders();
	InitSkyBox();
	InitFloor();
	InitBall();
	InitParticles();

	m_floorTexture.FromFile("assets/grass.jpg");
	m_ballTexture.FromFile("assets/ball.png");

	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	return true;
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		// elülsõ lap
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülsõ lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felsõ
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &m_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	TextureFromFileAttach("assets/xpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFileAttach("assets/xneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFileAttach("assets/ypos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFileAttach("assets/yneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFileAttach("assets/zpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFileAttach("assets/zneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void CMyApp::InitShaders()
{
	m_axesProgram.Init({
			{GL_VERTEX_SHADER,		"axes.vert"},
			{GL_FRAGMENT_SHADER,	"axes.frag"}
	});

	m_floorProgram.Init(
		{
			{ GL_VERTEX_SHADER, "floorVert.vert"},
			{ GL_FRAGMENT_SHADER, "floorFrag.frag"}
		},
	{
		{ 0, "vs_in_pos0" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_tex0" },				// VAO 2-es csatorna menjen a vs_in_tex-be
	}
	);

	m_ballProgram.Init(
		{
			{ GL_VERTEX_SHADER, "ballVert.vert"},
			{ GL_FRAGMENT_SHADER, "ballFrag.frag"}
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
			{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
		}
	);

	m_particleProgram.Init({	
		{ GL_VERTEX_SHADER,		"particle.vert" },
		{ GL_FRAGMENT_SHADER,	"particle.frag" }
		},
	{	
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_vel" },
	});

	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		}
	);
}

void CMyApp::InitFloor()
{
				
	m_floorPos.BufferData(
		std::vector<glm::vec3>{
			glm::vec3(0,  0, 0),	
			glm::vec3(10, 0, 0), 
			glm::vec3(0,  0, 10), 
			glm::vec3(10, 0, 10)  
	}
	);
	
	m_floorTex.BufferData(
		std::vector<glm::vec2>{
			glm::vec2(0, 0),
			glm::vec2(1, 0),
			glm::vec2(0, 1),
			glm::vec2(1, 1)
	}
	);

	m_floorIndices.BufferData(
		std::vector<int>{
		1, 3, 0,
		3, 2, 0
	}
	);
	

	m_floorVAO.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_floorPos },
			{ CreateAttribute<1, glm::vec2, 0, sizeof(glm::vec2)>, m_floorTex },
		}, m_floorIndices
	);

}

void CMyApp::InitBall()
{
	Vertex vert[(N + 1) * (M + 1)];
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j * (N + 1)].p = glm::vec3(u, v, 0);		
			vert[i + j * (N + 1)].t = GetTex(u, v);
		}

	GLushort indices[3 * 2 * (N) * (M)];
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1) * (N + 1);
		}

	m_ballVBO.BufferData(vert);

	m_ballIndices.BufferData(indices);

	m_ballVAO.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(Vertex)>, m_ballVBO },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_ballVBO },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_ballVBO },
		}, m_ballIndices
		);
}

void CMyApp::InitParticles() {

	glPointSize(5.0f);

	m_particlePos.reserve(m_particleCount);
	m_particleVel.reserve(m_particleCount);

	std::random_device rd; 
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rndPos(0, 10);
	std::uniform_real_distribution<> rndVel(-5, 5);

	for (int i = 0; i < m_particleCount; ++i)
	{
		m_particlePos.push_back(glm::vec3(rndPos(gen), 0, rndPos(gen)));
		m_particleVel.push_back(glm::vec3(rndVel(gen), 0, rndVel(gen)));
	}

	m_gpuParticleBuffer.BufferData(m_particlePos);
	m_gpuParticleVelBuffer.BufferData(m_particleVel);

	m_gpuParticleVAO.Init({
		{CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_gpuParticleBuffer},
		{CreateAttribute<1, glm::vec3, 0, sizeof(glm::vec3)>, m_gpuParticleVelBuffer}
		});
}

glm::vec2 CMyApp::GetTex(float u, float v)
{
	return glm::vec2(1 - u, 1 - v);
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_skyboxTexture);

}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();

	static float elapsedSinceSlowed = 0;
	elapsedSinceSlowed += delta_time;

	if ( elapsedSinceSlowed >= 1.0f) {
		m_camera.SetSpeed(m_camera.GetSpeed() * ( breakIsOn ? 0.65f : 0.95f));
		elapsedSinceSlowed -= 1.0f;
	}

	std::vector<glm::vec3> newParticlePositions{};
	std::vector<glm::vec3> newParticleVelocity{};
	newParticlePositions.reserve(m_particleCount);
	newParticlePositions.reserve(m_particleCount);

	for (int i = 0; i < m_particleCount; ++i)
	{

		m_particlePos[i] += m_particleVel[i] * delta_time;

		for (int j = 0; j < i; ++j) {
			if (abs(m_particlePos[i].x - m_particlePos[j].x) < particleSize) {
				m_particleVel[i].x *= -1;
				m_particleVel[j].x *= -1;
			}

			if (abs(m_particlePos[i].x - m_particlePos[j].x) < particleSize) {
				m_particleVel[i].z *= -1;
				m_particleVel[j].z *= -1;
			}
		}

		if ((m_particlePos[i].x >= 10 && m_particleVel[i].x > 0) || (m_particlePos[i].x <= 0 && m_particleVel[i].x < 0))
			m_particleVel[i].x *= -1; 
		if ((m_particlePos[i].z >= 10 && m_particleVel[i].z > 0) || (m_particlePos[i].z <= 0 && m_particleVel[i].z < 0))
			m_particleVel[i].z *= -1;
		
	}


	m_gpuParticleBuffer.BufferSubData(0, sizeof(glm::vec3) * m_particlePos.size(), &(m_particlePos[0][0]));


	if (ImGui::Begin("Presentation settings"))
	{
		if (ImGui::Button("Turn on/off axes"))
			axesShown = !axesShown;

		if (ImGui::Button("Back to the base")){
			m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			m_camera.SetSpeed(0.0f);
		}

		ImGui::Text("Camera velocity: %f ", m_camera.GetSpeed());

	}
	ImGui::End();
}

void CMyApp::RenderFloor()
{
	glm::mat4 viewProj = m_camera.GetViewProj();

	m_floorProgram.Use();
	m_floorProgram.SetTexture("texImage", 0, m_floorTexture);
	m_floorProgram.SetUniform("MVP", viewProj);
	
	m_floorVAO.Bind();
	glDrawElements(GL_TRIANGLES, 10, GL_UNSIGNED_SHORT, 0);
	m_floorVAO.Unbind();
	m_floorProgram.Unuse();
}

void CMyApp::RenderAxes() {
	m_axesProgram.Use();
	m_axesProgram.SetUniform("mvp", m_camera.GetViewProj());
	glDrawArrays(GL_LINES, 0, 6);
	m_axesProgram.Unuse();
}

void CMyApp::RenderBall(glm::mat4 viewProj)
{

	m_ballProgram.Use();
	m_ballProgram.SetTexture("texImage", 0, m_ballTexture);
	m_ballProgram.SetUniform("MVP", viewProj * glm::translate(glm::vec3(ballWorld)));
	m_ballProgram.SetUniform("world", glm::mat4(glm::translate(ballWorld)));
	m_ballProgram.SetUniform("worldIT", glm::inverse(glm::transpose(glm::mat4(glm::translate(ballWorld)))));

	m_ballVAO.Bind();
	glDrawElements(GL_TRIANGLES,	// primitív típus
		3 * 2 * (N) * (M),			// hany csucspontot hasznalunk a kirajzolashoz
		GL_UNSIGNED_SHORT,			// indexek tipusa
		0);							// indexek eltolása
	m_ballVAO.Unbind();
	m_ballProgram.Unuse();
}

void CMyApp::RenderParticles() {
	m_particleProgram.Use();
	glEnable(GL_PROGRAM_POINT_SIZE);
	m_gpuParticleVAO.Bind();
	m_particleProgram.Use();
	m_particleProgram.SetUniform("mvp", m_camera.GetViewProj());

	glDrawArrays(GL_POINTS, 0, m_particleCount);
	m_gpuParticleVAO.Unbind();
	glDisable(GL_PROGRAM_POINT_SIZE);
	m_particleProgram.Unuse();
}

void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	if (axesShown) RenderAxes();
	RenderFloor();
	RenderBall(viewProj);

	RenderParticles();
	
	RenderSkybox();

}

void CMyApp::RenderSkybox() {
	// skybox
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", m_camera.GetViewProj() * glm::translate(m_camera.GetEye()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	glDepthFunc(prevDepthFnc);
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{

	switch (key.keysym.sym)
	{
		case SDLK_RIGHT:
			ballWorld.z = ballWorld.z < 9.0f && (9.0f - ballWorld.z) > ballVelocity ? ballWorld.z + ballVelocity : ballWorld.z;
			break;
		case SDLK_LEFT:
			ballWorld.z = ballWorld.z > 1.0f && (ballWorld.z - 1.0f) > ballVelocity ? ballWorld.z - ballVelocity : ballWorld.z;
			break;
		case SDLK_DOWN:
			ballWorld.x = ballWorld.x > 1.0f && (ballWorld.x - 1.0f) > ballVelocity ? ballWorld.x - ballVelocity : ballWorld.x;
			break;
		case SDLK_UP:
			ballWorld.x = ballWorld.x < 9.0f && (9.0f - ballWorld.x) > ballVelocity ? ballWorld.x + ballVelocity : ballWorld.x;
			break;
		case SDLK_w: case SDLK_s: case SDLK_a: case SDLK_d:
			breakIsOn = false;
			m_camera.SetSpeed(1 + (m_camera.GetSpeed() * 1.1f));
			break;
		case SDLK_SPACE:
			breakIsOn = !breakIsOn;
			break;
		}
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{

	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
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

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}
