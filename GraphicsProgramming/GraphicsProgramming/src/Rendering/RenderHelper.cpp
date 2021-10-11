#include "RenderHelper.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


const RenderHelper::Vertex RenderHelper::cubeVertices[24] = {
	// front face
	{ { -0.5f,  0.5f,  0.5f }, { 0, 0, 1 } },
	{ {  0.5f,  0.5f,  0.5f }, { 0, 0, 1 } },
	{ {  0.5f, -0.5f,  0.5f }, { 0, 0, 1 } },
	{ { -0.5f, -0.5f,  0.5f }, { 0, 0, 1 } },

	// back face
	{ {  0.5f,  0.5f, -0.5f }, { 0, 0, -1 } },
	{ { -0.5f,  0.5f, -0.5f }, { 0, 0, -1 } },
	{ { -0.5f, -0.5f, -0.5f }, { 0, 0, -1 } },
	{ {  0.5f, -0.5f, -0.5f }, { 0, 0, -1 } },

	// left face
	{ { -0.5f,  0.5f, -0.5f }, { -1, 0, 0 } },
	{ { -0.5f,  0.5f,  0.5f }, { -1, 0, 0 } },
	{ { -0.5f, -0.5f,  0.5f }, { -1, 0, 0 } },
	{ { -0.5f, -0.5f, -0.5f }, { -1, 0, 0 } },

	// right face
	{ {  0.5f,  0.5f,  0.5f }, { 1, 0, 0 } },
	{ {  0.5f,  0.5f, -0.5f }, { 1, 0, 0 } },
	{ {  0.5f, -0.5f, -0.5f }, { 1, 0, 0 } },
	{ {  0.5f, -0.5f,  0.5f }, { 1, 0, 0 } },

	// top face
	{ { -0.5f,  0.5f, -0.5f }, { 0, 1, 0 } },
	{ {  0.5f,  0.5f, -0.5f }, { 0, 1, 0 } },
	{ {  0.5f,  0.5f,  0.5f }, { 0, 1, 0 } },
	{ { -0.5f,  0.5f,  0.5f }, { 0, 1, 0 } },

	// bottom face
	{ { -0.5f, -0.5f,  0.5f }, { 0, -1, 0 } },
	{ {  0.5f, -0.5f,  0.5f }, { 0, -1, 0 } },
	{ {  0.5f, -0.5f, -0.5f }, { 0, -1, 0 } },
	{ { -0.5f, -0.5f, -0.5f }, { 0, -1, 0 } }
};
const size_t RenderHelper::cubeIndices[36] = {
	// front face
	0, 1, 2,
	0, 2, 3,
	// back face
	4, 5, 6,
	4, 6, 7,
	// left face
	8, 9, 10,
	8, 10, 11,
	// right face
	12, 13, 14,
	12, 14, 15,
	// top face
	16, 17, 18,
	16, 18, 19,
	// bottom face
	20, 21, 22,
	20, 22, 23
};


void RenderHelper::drawSphere(float radius, int slices, int stacks)
{
	glutSolidSphere(radius, slices, stacks);
}

void RenderHelper::drawUnitCube()
{
	glBegin(GL_TRIANGLES);

	for (auto index : cubeIndices)
	{
		auto vertex = cubeVertices[index];

		auto p = vertex.position;
		auto n = vertex.normal;
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(p.x, p.y, p.z);
	}

	glEnd();
}

void RenderHelper::drawMesh(const Mesh& mesh)
{
	glBegin(GL_TRIANGLES);

	for (auto i : mesh.Indices)
	{
		const auto& p = mesh.Vertices[i];
		const auto& n = mesh.Normals[i];
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(p.x, p.y, p.z);
	}

	glEnd();
}

