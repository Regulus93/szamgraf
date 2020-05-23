#pragma once

// SDL
#include <SDL.h>
#include <SDL_opengl.h>
#include "Vertex.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class Cylinder
{
public:
	Cylinder() 
	{

	}

	~Cylinder(void);

	void InitVertexArray(Vertex vert[], int N, int M);
	void Cylinder::InitIndexArray(GLushort indices[], int N, int M);

	static const int N = 20;
	static const int M = 10;

	static const int cylinder_evaluation_count = (N + 1) * (M + 1);
	static const int cylinder_indices_count = 3 * 2 * (N) * (M);

	float cylinder_height = 3.0f;
	float cylinder_radius = 1.0f;

	static const int circle_evaluation_count = N + 1 + 2;

	static const int circle_start_vert_ind = cylinder_evaluation_count;
	static const int circle_end_vert_ind = cylinder_evaluation_count + circle_evaluation_count;
	static const int circle_start_indices_ind = cylinder_indices_count;
	static const int circle_end_indices_ind = cylinder_indices_count + circle_evaluation_count;

protected: 
	glm::vec3 GetVec3PointsOfCylinderSurfaceFrom2Dimension(float u, float v);
	glm::vec3 GetVec3PointsOfCylinderCircle(float t);

};
