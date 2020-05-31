#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

void CMyApp::InitCube()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex>vertices;
	vertices.reserve(6 * 4);
	
	//front									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 1) });
	//back
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 1) });
	//right									 
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 1) });
	//left									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 1) });
	//top									 
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1) });
	//bottom								 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, -1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, -1, 0), glm::vec2(1, 1) });

	std::vector<GLuint> indices(36);
	size_t index = 0;
	for (int i = 0; i < 6 * 4; i += 4)
	{
		indices[index + 0] = i + 0;
		indices[index + 1] = i + 1;
		indices[index + 2] = i + 2;
		indices[index + 3] = i + 1;
		indices[index + 4] = i + 3;
		indices[index + 5] = i + 2;
		index += 6;
	}

	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe való feltöltése BufferData-val
	//

	// vertexek pozíciói:
	/*
	Az m_CubeVertexBuffer konstruktora már létrehozott egy GPU puffer azonosítót és a most következő BufferData hívás ezt
	1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_CubeVertexBuffer típusa ArrayBuffer) és
	2. glBufferData segítségével áttölti a GPU-ra az argumentumban adott tároló értékeit

	*/

	m_CubeVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_CubeIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_CubeVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			//{ CreateAttribute<	0,						// attribútum: 0
			//						glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
			//						0,						// offset: az attribútum tároló elejétől vett offset-je, byte-ban
			//						sizeof(Vertex)			// stride: a következő csúcspont ezen attribútuma hány byte-ra van az aktuálistól
			//					>, m_CubeVertexBuffer },
			{ CreateAttribute<0, glm::vec3, 0,						 sizeof(Vertex)>, m_CubeVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)),	 sizeof(Vertex)>, m_CubeVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
		},
		m_CubeIndices
	);
}

glm::vec3 GetSpherePos(float u, float v)
{
	u *= 2 * float(M_PI);
	v *= float(M_PI);
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	return glm::vec3(cu * sv, cv, su * sv);
}
glm::vec3 GetSphereNorm(float u, float v)
{
	u *= 2 * float(M_PI);
	v *= float(M_PI);
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	return glm::vec3(cu * sv, cv, su * sv);
}
glm::vec2 GetSphereTex(float u, float v)
{
	return glm::vec2(1 - u, 1 - v);
}
void CMyApp::InitSphere()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex>vertices((N + 1) * (M + 1));
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vertices[i + j * (N + 1)].p = GetSpherePos(u, v);
			vertices[i + j * (N + 1)].n = GetSphereNorm(u, v);
			vertices[i + j * (N + 1)].t = GetSphereTex(u, v);
		}

	std::vector<GLuint> indices(3 * 2 * (N) * (M));
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

	m_SphereVertexBuffer.BufferData(vertices);
	m_SphereIndices.BufferData(indices);
	m_SphereVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0,						sizeof(Vertex)>, m_SphereVertexBuffer },
			{ CreateAttribute<1, glm::vec3,	(sizeof(glm::vec3)),	sizeof(Vertex)>, m_SphereVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)),sizeof(Vertex)>, m_SphereVertexBuffer },
		},
		m_SphereIndices
		);
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
		glm::vec3( 1, -1, -1),
		glm::vec3( 1,  1, -1),
		glm::vec3(-1,  1, -1),
		// elülső lap
		glm::vec3(-1, -1,  1),
		glm::vec3( 1, -1,  1),
		glm::vec3( 1,  1,  1),
		glm::vec3(-1,  1,  1),
	}
	);

	m_SkyboxIndices.BufferData(
		std::vector<GLuint>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülső lap
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
			// felső
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

glm::vec3 CMyApp::GetGroundPos(float u, float v)
{
	return glm::vec3(u * 40.0f - 20.f, 0, v * 40.0f - 20.0f);
}

void CMyApp::InitShaders()
{
	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_MeshProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_MeshProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
		});

	m_MeshProgram.LinkProgram();

	// shader program rövid létrehozása, egyetlen függvényhívással a fenti három:
	m_SkyboxProgram.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		}
		);

	// shader program létrehozása még rövidebben:
	// { GL_VERTEX_SHADER, "shader.vert" } helyett "shader.vert"_vs
	m_AxesProgram.Init({ "axes.vert"_vs, "axes.frag"_fs });

	groundProgram.Init(
		{
			{ GL_VERTEX_SHADER, "ground.vert" },
			{ GL_FRAGMENT_SHADER, "ground.frag" }
		},
		{
			{ 0, "vs_in_pos" },
			{ 1, "vs_in_norm" },
		}
	);

	cubeProgram.Init(
		{
			{ GL_VERTEX_SHADER, "cube.vert" },
			{ GL_FRAGMENT_SHADER, "cube.frag" }
		},
		{
			{ 0, "vs_in_pos" },
			{ 1, "vs_in_norm" },
			{ 2, "vs_in_tex" },
		}
		);

	sphereProgram.Init(
		{
			{ GL_VERTEX_SHADER, "sphere.vert" },
			{ GL_FRAGMENT_SHADER, "sphere.frag" }
		},
		{
			{ 0, "vs_in_pos" },
			{ 1, "vs_in_norm" },
			{ 2, "vs_in_tex" },
		}
		);

}

void CMyApp::InitGround() {

	std::vector<Vertex> vertices;
	vertices.reserve((groundN + 1) * (groundM + 1));

	for (int i = 0; i <= groundN; ++i)
		for (int j = 0; j <= groundM; ++j)
		{
			float u = i / (float)groundN;
			float v = j / (float)groundM;

			vertices.push_back({ GetGroundPos(u, v), glm::vec3(0.5f), glm::vec2(0.25f)});
		}

	groundVbo.BufferData(vertices);

	GLushort indices[3 * 2 * (200) * (200)];

	for (int i = 0; i < groundN; ++i)
		for (int j = 0; j < groundM; ++j)
		{
			indices[6 * i + j * 3 * 2 * (groundN)+0] = (i)+(j) * (groundN + 1);
			indices[6 * i + j * 3 * 2 * (groundN)+1] = (i + 1) + (j) * (groundN + 1);
			indices[6 * i + j * 3 * 2 * (groundN)+2] = (i)+(j + 1) * (groundN + 1);
			indices[6 * i + j * 3 * 2 * (groundN)+3] = (i + 1) + (j) * (groundN + 1);
			indices[6 * i + j * 3 * 2 * (groundN)+4] = (i + 1) + (j + 1) * (groundN + 1);
			indices[6 * i + j * 3 * 2 * (groundN)+5] = (i)+(j + 1) * (groundN + 1);
		}

	groundInd.BufferData(indices);


	groundVao.Init(
	{
		{ CreateAttribute<0, glm::vec3, 				0,	sizeof(Vertex)>, groundVbo },
		{ CreateAttribute<1, glm::vec3, sizeof(glm::vec3),	sizeof(Vertex)>, groundVbo },
	}, groundInd
	);
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafelé nező lapok eldobását
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	InitShaders();
	InitSkyBox();
	InitCube();
	InitSphere();
	InitGround();
	

	// egyéb textúrák betöltése
	m_WoodTexture.FromFile("assets/wood.jpg");
	m_MarronTexture.FromFile("assets/marron.jpg");
	m_EarthTexture.FromFile("assets/earth.jpg");

	taylorDressTexture.FromFile("assets/dress.jpg");
	taylorTexture.FromFile("assets/taylor.jpg");
	dancerTexture.FromFile("assets/dancer.jpg");
	dancerDressTexture.FromFile("assets/dancerDress.jpg");

	// mesh betöltése
	m_SuzanneMesh = ObjParser::parse("assets/Suzanne.obj");
	m_SuzanneMesh->initBuffers();
	
	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_skyboxTexture);

	delete m_SuzanneMesh;
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::RenderGround()
{
	glm::mat4 viewProj = m_camera.GetViewProj();

	groundProgram.Use();
	groundProgram.SetUniform("MVP", viewProj);

	groundVao.Bind();
	glDrawElements(GL_TRIANGLES,	// primitív típus
		3 * 2 * (groundN) * (groundN),			// hany csucspontot hasznalunk a kirajzolashoz
		GL_UNSIGNED_SHORT,			// indexek tipusa
		0);							// indexek eltolása
	groundVao.Unbind();
	groundProgram.Unuse();
}

void CMyApp::RenderStage() {
	
	glm::mat4 stageWorld = glm::translate(glm::vec3(0.f,1.f,0.f)) * glm::scale<float>(glm::vec3(10.f,2.f,7.f));
	cubeProgram.Use();
	cubeProgram.SetUniform("coloring", 1);

	RenderCube(stageWorld);
}

void CMyApp::RenderCube(glm::mat4 world) {

	glm::mat4 viewProj = m_camera.GetViewProj();

	cubeProgram.Use();

	cubeProgram.SetUniform("MVP", viewProj * world);
	cubeProgram.SetUniform("world", world);
	cubeProgram.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

	m_CubeVao.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	m_CubeVao.Unbind();

	cubeProgram.Unuse();
}

void CMyApp::RenderSphere(glm::mat4 world) {

	glm::mat4 viewProj = m_camera.GetViewProj();

	sphereProgram.Use();

	sphereProgram.SetUniform("MVP", viewProj * world);
	sphereProgram.SetUniform("world", world);
	sphereProgram.SetUniform("worldIT", glm::inverse(glm::transpose(world)));

	m_SphereVao.Bind();
	glDrawElements(GL_TRIANGLES, N * M * 6, GL_UNSIGNED_INT, nullptr);
	m_SphereVao.Unbind();

	sphereProgram.Unuse();
}

void CMyApp::RenderPerson(glm::vec3 position, bool isTaylor) {
	RenderBody(position,isTaylor);
	RenderHead(position,isTaylor);

	if (!isTaylor) {
		position.x = position.x * -1;
		RenderBody(position, isTaylor);
		RenderHead(position, isTaylor);
	}
}

void CMyApp::RenderBody(glm::vec3 position, bool isTaylor) {

	glm::mat4 world = glm::translate(position) * glm::scale<float>(glm::vec3(0.5f,1.f,0.5f));
	cubeProgram.Use();
	cubeProgram.SetUniform("coloring", 2);
	cubeProgram.SetTexture("texImage2", 1, isTaylor ? taylorDressTexture : dancerDressTexture);

	RenderCube(world);
}

void CMyApp::RenderHead(glm::vec3 position, bool isTaylor) {
	glm::mat4 world = glm::translate(glm::vec3(position.x, position.y+0.75f, position.z)) * glm::scale<float>(glm::vec3(0.25f, 0.25f, 0.25f));
	cubeProgram.Use();
	cubeProgram.SetTexture("texImage", 0, isTaylor ? taylorTexture : dancerTexture);

	RenderSphere(world);

}


/*
Util function to do this:

	RenderPerson(glm::vec3(-1.f, sinf((last_time / 1000.0f) * 0.9f) * 3.0f + 1.5f + 3.f, 1.f),false);

	- sinf((last_time / 1000.0f) * 0.9f)						=> period is translated with 0.9f
	- sinf((last_time / 1000.0f) * 0.9f) * 3.0f					=> value of the sin increased from (-1.0f,1.0f) to (-3.0f,3.0f)
	- sinf((last_time / 1000.0f) * 0.9f) * 3.0f + 1.5f + 3.f	=> value of the sin translated from (-3.0f,3.0f) to (1.5f,7.5f)

*/
float GetPersonYPosition(Uint32 time, float periodChange) {

	float valueOfSin = sinf(time / 1000.0f * (1.0f - periodChange));

	float highOfStage = 2.5f;
	float intervalMax = 3.f;
	float intervalToPositive = 3.f;

	return valueOfSin * intervalMax + highOfStage + intervalToPositive;
}

void CMyApp::Render()
{
	// töröljük a framebuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z buffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	RenderGround();
	RenderStage();

	Uint32 renderTime = SDL_GetTicks();

	//TaylorSwift
	RenderPerson(glm::vec3(0.f, GetPersonYPosition(renderTime, 0.f), 2.f), true);

	//1. vonal
	RenderPerson(glm::vec3(-1.f, GetPersonYPosition(renderTime, -0.05f), 1.f),false);
	
	//2. vonal
	RenderPerson(glm::vec3(-1.5f, GetPersonYPosition(renderTime, -0.10f), 0.f), false);

	//3. vonal
	RenderPerson(glm::vec3(-2.f, GetPersonYPosition(renderTime, -0.15f), -1.f), false);





	// tengelyek
	m_AxesProgram.Use();
	m_AxesProgram.SetUniform("mvp", viewProj);
	glDrawArrays(GL_LINES, 0, 6);
	m_AxesProgram.Unuse();

	// skybox
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
	glDepthFunc(GL_LEQUAL);

	m_SkyboxProgram.Use();
	m_SkyboxProgram.SetUniform("MVP", viewProj * glm::translate( m_camera.GetEye()) );
	m_SkyboxProgram.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);
	m_SkyboxVao.Bind();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_SkyboxVao.Unbind();
	m_SkyboxProgram.Unuse();

	glDepthFunc(prevDepthFnc);
	
	// ImGui demo ablak
	//ImGui::ShowTestWindow();

	ImGui::SetNextWindowPos(ImVec2(300, 400), ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin("Tesztablak"))
	{
		ImGui::Text("Tesztszoveg");
	}
	ImGui::End();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
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

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}
