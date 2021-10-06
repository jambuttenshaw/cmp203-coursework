#include "RenderHelper.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include <iostream>


const RenderHelper::Vertex RenderHelper::cubeVertices[24] = {
	// front face
	{ { -1,  1,  1 }, { 0, 0, 1 } },
	{ {  1,  1,  1 }, { 0, 0, 1 } },
	{ {  1, -1,  1 }, { 0, 0, 1 } },
	{ { -1, -1,  1 }, { 0, 0, 1 } },

	// back face
	{ {  1,  1, -1 }, { 0, 0, -1 } },
	{ { -1,  1, -1 }, { 0, 0, -1 } },
	{ { -1, -1, -1 }, { 0, 0, -1 } },
	{ {  1, -1, -1 }, { 0, 0, -1 } },

	// left face
	{ { -1,  1, -1 }, { -1, 0, 0 } },
	{ { -1,  1,  1 }, { -1, 0, 0 } },
	{ { -1, -1,  1 }, { -1, 0, 0 } },
	{ { -1, -1, -1 }, { -1, 0, 0 } },

	// right face
	{ {  1,  1,  1 }, { 1, 0, 0 } },
	{ {  1,  1, -1 }, { 1, 0, 0 } },
	{ {  1, -1, -1 }, { 1, 0, 0 } },
	{ {  1, -1,  1 }, { 1, 0, 0 } },

	// top face
	{ { -1,  1, -1 }, { 0, 1, 0 } },
	{ {  1,  1, -1 }, { 0, 1, 0 } },
	{ {  1,  1,  1 }, { 0, 1, 0 } },
	{ { -1,  1,  1 }, { 0, 1, 0 } },

	// bottom face
	{ { -1, -1,  1 }, { 0, -1, 0 } },
	{ {  1, -1,  1 }, { 0, -1, 0 } },
	{ {  1, -1, -1 }, { 0, -1, 0 } },
	{ { -1, -1, -1 }, { 0, -1, 0 } }
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


void RenderHelper::drawSphere(float size, int slices, int stacks)
{
	gluSphere(gluNewQuadric(), size, slices, stacks);
}

void RenderHelper::drawSphere(float size, float r, float g, float b, int slices, int stacks)
{
	glColor3f(r, g, b);
	drawSphere(size, slices, stacks);
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

