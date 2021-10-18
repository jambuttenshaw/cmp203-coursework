#include "RenderHelper.h"

#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>


void RenderHelper::drawSphere(float radius, int slices, int stacks)
{
	glutSolidSphere(radius, slices, stacks);
}

void RenderHelper::drawQuad()
{
	drawMesh(Mesh::Quad);
}

void RenderHelper::drawUnitCube()
{
	drawMesh(Mesh::Cube);
}

void RenderHelper::drawMesh(const Mesh& mesh)
{
	glBegin(GL_TRIANGLES);

	for (auto i : mesh.Indices)
	{
		const auto& p = mesh.Vertices[i];
		const auto& n = mesh.Normals[i];
		const auto& uv = mesh.TexCoords[i];
		glTexCoord2f(uv.x, uv.y);
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(p.x, p.y, p.z);
	}

	glEnd();
}

