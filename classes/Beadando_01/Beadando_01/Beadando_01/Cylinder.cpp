#include "Cylinder.h"

Cylinder::~Cylinder(void)
{
}

glm::vec3 Cylinder::GetVec3PointsOfCylinderSurfaceFrom2Dimension(float u, float v)
{
	u *= 2 * 3.1415f;
	v *= cylinder_height;

	float cu = cosf(u);
	float su = sinf(u);

	return glm::vec3(cylinder_radius * su, v, cylinder_radius * cu);
}

glm::vec3 Cylinder::GetVec3PointsOfCylinderCircle(float t)
{
	t *= 2 * 3.1415f;
	float ct = cosf(t);
	float st = sinf(t);

	return glm::vec3(cylinder_radius * st, 0, cylinder_radius * ct);
}

void Cylinder::InitVertexArray(Vertex vert[], int N, int M)
{
	for (int i = 0; i <= N; ++i) {
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j * (N + 1)].p = GetVec3PointsOfCylinderSurfaceFrom2Dimension(u, v);
			vert[i + j * (N + 1)].c = glm::normalize(glm::vec3( sinf(i) * vert[i].p.x + N/M, cosf(vert[i].p.y) + i*j/N, i * j / M + vert[i].p.z));		}
	}

	vert[Cylinder::circle_start_vert_ind].p = glm::vec3(0, 0, 0);
	vert[Cylinder::circle_start_vert_ind].c = glm::vec3(0.5, 0.5, 0.5);
	for (int i = Cylinder::circle_start_vert_ind + 1; i < Cylinder::circle_end_vert_ind; ++i)
	{
		float t = i / (float)Cylinder::N;
		vert[i].p = GetVec3PointsOfCylinderCircle(t);
		glm::vec3 np = glm::normalize(glm::vec3(sinf(i) * vert[i].p.x + N / M, i/vert[i].p.y, vert[i].p.z));
		vert[i].c = np;
	}
}

/*
	A pontrácsunkon egy vertex több háromszöghöz tartozik.
	A belsõ pontoknál: X pont => A, B, C, D, E, F háromszögek csúcspontja egyszerre

	|-----|-----|
	|\    |\    |
	| \   | \   |
	|  \C |D \  |
	|   \ |   \ |
	| B  \|    \|
	|-----X-----|
	|\ A  |\ E  |
	| \   | \   |
	|  \  |F \  |
	|   \ |   \ |
	|    \|    \|
	|-----------|

	Egy négyzet két háromszögbõl áll, és a két háromszög is osztozik két-két csúcsponton:
	B,E illetve D,F

	B,E---C				B	          E-----C
	|\    |				|\             \    |
	| \   |		=>		| \             \   |
	|  \  |		=>		|  \    és       \  |
	|   \ |		=>		|   \             \ |
	|    \|				|    \             \|
	A-----D,F			A-----D				F

*/
void Cylinder::InitIndexArray(GLushort indices[], int N, int M) 
{
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < M; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);				//A csúcs
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j) * (N + 1);		//B csúcs 
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1) * (N + 1);         //D csúcs
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);       //E csúcs
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1) * (N + 1);   //C csúcs
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1) * (N + 1);			//F csúcs
		}
	}

	int index = Cylinder::circle_start_vert_ind;
	for (int i = Cylinder::circle_start_indices_ind; i < Cylinder::circle_end_indices_ind; ++i)
	{
		indices[i] = index;
		++index;
	}
}
